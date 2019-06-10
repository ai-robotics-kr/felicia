/*
 *  RPLIDAR ROS NODE
 *
 *  Copyright (c) 2009 - 2014 RoboPeak Team
 *  http://www.robopeak.com
 *  Copyright (c) 2014 - 2016 Shanghai Slamtec Co., Ltd.
 *  http://www.slamtec.com
 *
 */
/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "felicia/drivers/vendors/rplidar/rplidar.h"

#include "third_party/chromium/base/bind.h"
#include "third_party/chromium/base/memory/ptr_util.h"
#include "third_party/chromium/base/stl_util.h"
#include "third_party/chromium/base/strings/strcat.h"
#include "third_party/chromium/base/strings/stringprintf.h"

#include "felicia/core/lib/error/errors.h"

using namespace rp::standalone::rplidar;

namespace felicia {

#define DEG2RAD(x) ((x)*M_PI / 180.)
#define WITH_RESULT(text, result) ::base::StringPrintf("%s: %X", text, result)

RPlidar::RPlidar(const LidarEndpoint& lidar_endpoint)
    : LidarInterface(lidar_endpoint), thread_("RPlidarThread") {}

RPlidar::~RPlidar() = default;

Status RPlidar::Init() {
  if (!lidar_state_.IsStopped()) {
    return lidar_state_.InvalidStateError();
  }

  std::unique_ptr<RPlidarDriver> driver;
  if (lidar_endpoint_.type() == LidarEndpoint::TCP) {
    driver = ::base::WrapUnique(RPlidarDriver::CreateDriver(DRIVER_TYPE_TCP));
  } else {
    driver =
        ::base::WrapUnique(RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT));
  }

  if (!driver) {
    return errors::Unavailable("Failed to create driver");
  }

  u_result result;
  if (lidar_endpoint_.type() == LidarEndpoint::TCP) {
    result = driver->connect(lidar_endpoint_.ip().c_str(),
                             lidar_endpoint_.tcp_port());
  } else {
    result = driver->connect(lidar_endpoint_.serial_port().c_str(),
                             lidar_endpoint_.buadrate());
  }

  if (IS_FAIL(result)) {
    return errors::Unavailable(::base::StrCat(
        {"Failed to connect to the endpoint", lidar_endpoint_.ToString()}));
  }

  rplidar_response_device_info_t devinfo;
  result = driver->getDeviceInfo(devinfo);
  if (IS_OK(result)) {
    std::string text;
    ::base::StringAppendF(&text, "%s", "RPLIDAR S/N: ");
    for (int i = 0; i < 16; ++i) {
      ::base::StringAppendF(&text, "%02X", devinfo.serialnum[i]);
    }
    ::base::StringAppendF(&text, "\nFirmware Ver: %d.%02d\nHardware Rev: %d",
                          devinfo.firmware_version >> 8,
                          devinfo.firmware_version & 0xFF,
                          devinfo.hardware_version);
    LOG(INFO) << text;
  } else {
    return errors::Unavailable(WITH_RESULT("Failed to getDeviceInfo", result));
  }

  rplidar_response_device_health_t healthinfo;
  result = driver->getHealth(healthinfo);
  if (IS_OK(result)) {
    if (healthinfo.status == RPLIDAR_STATUS_WARNING) {
      LOG(WARNING) << "getHealth " << healthinfo.error_code;
    } else if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
      return errors::Unavailable(
          ::base::StringPrintf("rplidar internal error detected (%x). Please "
                               "reboot the device to retry.",
                               healthinfo.error_code));
    }
  } else {
    return errors::Unavailable(WITH_RESULT("Failed to getHealth", result));
  }

  driver_ = std::move(driver);

  lidar_state_.ToInitialized();
  return Status::OK();
}

Status RPlidar::Start(LidarFrameCallback lidar_frame_callback) {
  return DoStart(nullptr, lidar_frame_callback);
}

Status RPlidar::Start(RplidarScanMode scan_mode,
                      LidarFrameCallback lidar_frame_callback) {
  return DoStart(&scan_mode, lidar_frame_callback);
}

Status RPlidar::Stop() {
  if (!lidar_state_.IsStarted()) {
    return lidar_state_.InvalidStateError();
  }

  is_stopping_ = true;

  if (thread_.IsRunning()) thread_.Stop();

  u_result result = driver_->stop();
  if (IS_FAIL(result)) {
    return errors::Unavailable(WITH_RESULT("Failed to stop", result));
  }
  result = driver_->stopMotor();
  if (IS_FAIL(result)) {
    return errors::Unavailable(WITH_RESULT("Failed to stopMotor", result));
  }

  lidar_state_.ToStopped();
  return Status::OK();
}

Status RPlidar::GetSupportedScanModes(
    std::vector<RplidarScanMode>* scan_modes) {
  DCHECK(scan_modes->empty());
  if (!lidar_state_.IsInitialized()) {
    return lidar_state_.InvalidStateError();
  }

  u_result result = driver_->getAllSupportedScanModes(*scan_modes);
  if (IS_FAIL(result)) {
    return errors::Unavailable(
        WITH_RESULT("Failed to getAllSupportedScanModes", result));
  }
  return Status::OK();
}

void RPlidar::DoScanOnce() {
  if (thread_.task_runner()->BelongsToCurrentThread()) {
    DoScan();
  } else {
    thread_.task_runner()->PostTask(
        FROM_HERE, ::base::BindOnce(&RPlidar::DoScan, AsWeakPtr()));
  }
}

void RPlidar::DoScanLoop() {
  if (thread_.task_runner()->BelongsToCurrentThread()) {
    DoScan();
  }

  if (!is_stopping_) {
    thread_.task_runner()->PostTask(
        FROM_HERE, ::base::BindOnce(&RPlidar::DoScanLoop, AsWeakPtr()));
  }
}

Status RPlidar::DoStart(RplidarScanMode* scan_mode,
                        LidarFrameCallback lidar_frame_callback) {
  if (!lidar_state_.IsInitialized()) {
    return lidar_state_.InvalidStateError();
  }

  u_result result = driver_->startMotor();
  if (IS_FAIL(result)) {
    return errors::Unavailable(WITH_RESULT("Failed to startMotor", result));
  }

  if (scan_mode) {
    result = driver_->startScanExpress(false /* not force scan */,
                                       scan_mode->id, 0, &scan_mode_);
  } else {
    result =
        driver_->startScan(false /* not force scan */,
                           true /* use typical scan mode */, 0, &scan_mode_);
  }

  if (IS_FAIL(result)) {
    return errors::Unavailable(WITH_RESULT("Failed to startScan", result));
  }

  lidar_frame_callback_ = lidar_frame_callback;
  thread_.Start();
  lidar_state_.ToStarted();
  return Status::OK();
}

namespace {

float Angle(const rplidar_response_measurement_node_hq_t& node) {
  return node.angle_z_q14 * 90.f / 16384.f;
}

}  // namespace

void RPlidar::DoScan() {
  DCHECK(thread_.task_runner()->BelongsToCurrentThread());
  rplidar_response_measurement_node_hq_t nodes[360 * 8];
  size_t count = ::base::size(nodes);

  ::base::TimeTicks start_scan_time = ::base::TimeTicks::Now();
  u_result result = driver_->grabScanDataHq(nodes, count);
  if (result != RESULT_OK) return;
  ::base::TimeTicks end_scan_time = ::base::TimeTicks::Now();
  double scan_time = (end_scan_time - start_scan_time).InSecondsF();

  result = driver_->ascendScanData(nodes, count);
  float angle_start, angle_end;
  if (result == RESULT_OK) {
    auto start_it =
        std::find_if(std::begin(nodes), std::end(nodes),
                     [](rplidar_response_measurement_node_hq_t& node) {
                       return node.dist_mm_q2 == 0;
                     });
    if (start_it == std::end(nodes)) {
      LOG(ERROR) << "Failed to find the first valid node";
      return;
    }
    auto end_it =
        std::find_if(std::rbegin(nodes), std::rend(nodes),
                     [](rplidar_response_measurement_node_hq_t& node) {
                       return node.dist_mm_q2 == 0;
                     });
    if (end_it == std::rend(nodes)) {
      LOG(ERROR) << "Failed to find the last valid node";
      return;
    }
    auto start_idx = std::distance(std::begin(nodes), start_it);
    auto end_idx = std::distance(std::rbegin(nodes), end_it);
    angle_start = DEG2RAD(Angle(*start_it));
    angle_end = DEG2RAD(Angle(*end_it));
    count = count - (start_idx + end_idx);
  } else {
    // All the data is invalid
    angle_start = DEG2RAD(0.0f);
    angle_end = DEG2RAD(359.0f);
  }

  LidarFrame lidar_frame;
  lidar_frame.set_angle_start(angle_start);
  lidar_frame.set_angle_end(angle_end);
  if (count > 1) {
    lidar_frame.set_angle_delta((angle_end - angle_start) /
                                static_cast<float>(count - 1));
  }
  lidar_frame.set_scan_time(scan_time);
  if (count > 1) {
    lidar_frame.set_time_delta(scan_time / static_cast<float>(count - 1));
  }
  lidar_frame.set_range_min(0.15);
  lidar_frame.set_range_max(scan_mode_.max_distance);

  lidar_frame.intensities().resize(count);
  lidar_frame.ranges().resize(count);
  for (size_t i = 0; i < count; i++) {
    float read_value = static_cast<float>(nodes[i].dist_mm_q2 / 4.0f / 1000);
    if (read_value == 0.0) {
      lidar_frame.ranges()[i] = std::numeric_limits<float>::infinity();
    } else {
      lidar_frame.ranges()[i] = read_value;
    }
    lidar_frame.intensities()[i] = static_cast<float>(nodes[i].quality >> 2);
  }
  lidar_frame.set_timestamp(timestamper_.timestamp());

  lidar_frame_callback_.Run(lidar_frame);
}

#undef DEG2RAD
#undef WITH_RESULT

}  // namespace felicia