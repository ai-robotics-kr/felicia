// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_DRIVERS_IMU_IMU_FRAME_H_
#define FELICIA_DRIVERS_IMU_IMU_FRAME_H_

#include "third_party/chromium/base/callback.h"

#include "felicia/core/lib/base/export.h"
#include "felicia/core/lib/error/status.h"
#include "felicia/core/lib/unit/geometry/quaternion.h"
#include "felicia/core/lib/unit/geometry/vector.h"
#include "felicia/drivers/imu/imu_frame_message.pb.h"

namespace felicia {
namespace drivers {

class FEL_EXPORT ImuFrame {
 public:
  ImuFrame();
  ImuFrame(const Quaternionf& orientation, const Vector3f& angular_velocity,
           const Vector3f& linear_acceleration, base::TimeDelta timestamp);

  void set_orientation(float w, float x, float y, float z);
  void set_angular_velocity(float x, float y, float z);
  void set_linear_acceleration(float x, float y, float z);

  void set_orientation(const Quaternionf& orientation);
  void set_angular_velocity(const Vector3f& angular_velocity);
  void set_linear_acceleration(const Vector3f& linear_acceleration);

  const Quaternionf& orientation() const;
  const Vector3f& angular_velocity() const;
  const Vector3f& linear_acceleration() const;

  void set_timestamp(base::TimeDelta timestamp);
  base::TimeDelta timestamp() const;

  ImuFrameMessage ToImuFrameMessage() const;
  Status FromImuFrameMessage(const ImuFrameMessage& message);

 private:
  Quaternionf orientation_;
  Vector3f angular_velocity_;
  Vector3f linear_acceleration_;
  base::TimeDelta timestamp_;
};

typedef base::RepeatingCallback<void(const ImuFrame&)> ImuFrameCallback;

}  // namespace drivers
}  // namespace felicia

#endif  // FELICIA_DRIVERS_IMU_IMU_FRAME_H_