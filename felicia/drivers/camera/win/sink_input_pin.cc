// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Modified by Wonyong Kim(chokobole33@gmail.com)
// Followings are taken and modified from
// https://github.com/chromium/chromium/blob/5db095c2653f332334d56ad739ae5fe1053308b1/media/capture/video/win/sink_input_pin_win.cc

#include "felicia/drivers/camera/win/sink_input_pin.h"

#include <cstring>

// Avoid including strsafe.h via dshow as it will cause build warnings.
#define NO_DSHOW_STRSAFE
#include <dshow.h>
#include <stdint.h>

#include "third_party/chromium/base/logging.h"
#include "third_party/chromium/base/stl_util.h"
#include "third_party/chromium/base/strings/stringprintf.h"

#include "felicia/drivers/camera/camera_frame_util.h"
#include "felicia/drivers/camera/timestamp_constants.h"

namespace felicia {

const REFERENCE_TIME kSecondsToReferenceTime = 10000000;

static DWORD GetArea(const BITMAPINFOHEADER& info_header) {
  return info_header.biWidth * info_header.biHeight;
}

SinkInputPin::SinkInputPin(IBaseFilter* filter, SinkFilterObserver* observer)
    : PinBase(filter), requested_frame_rate_(0), observer_(observer) {}

void SinkInputPin::SetRequestedMediaFormat(
    CameraFormat::PixelFormat pixel_format, float frame_rate,
    const BITMAPINFOHEADER& info_header) {
  requested_pixel_format_ = pixel_format;
  requested_frame_rate_ = frame_rate;
  requested_info_header_ = info_header;
  resulting_format_ = CameraFormat(0, 0, CameraFormat::PIXEL_FORMAT_UNKNOWN);
}

bool SinkInputPin::IsMediaTypeValid(const AM_MEDIA_TYPE* media_type) {
  const GUID type = media_type->majortype;
  if (type != MEDIATYPE_Video) return false;

  const GUID format_type = media_type->formattype;
  if (format_type != FORMAT_VideoInfo) return false;

  // Check for the sub types we support.
  const GUID sub_type = media_type->subtype;
  VIDEOINFOHEADER* pvi =
      reinterpret_cast<VIDEOINFOHEADER*>(media_type->pbFormat);
  if (pvi == NULL) return false;

  // Store the incoming width and height.
  resulting_format_.SetSize(pvi->bmiHeader.biWidth,
                            abs(pvi->bmiHeader.biHeight));
  if (pvi->AvgTimePerFrame > 0) {
    resulting_format_.set_frame_rate(
        static_cast<int>(kSecondsToReferenceTime / pvi->AvgTimePerFrame));
  } else {
    resulting_format_.set_frame_rate(requested_frame_rate_);
  }
  if (sub_type == kMediaSubTypeI420 &&
      pvi->bmiHeader.biCompression == MAKEFOURCC('I', '4', '2', '0')) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_I420);
    return true;
  }
  if (sub_type == MEDIASUBTYPE_YUY2 &&
      pvi->bmiHeader.biCompression == MAKEFOURCC('Y', 'U', 'Y', '2')) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_YUY2);
    return true;
  }
  // This format is added after http:/crbug.com/508413.
  if (sub_type == MEDIASUBTYPE_UYVY &&
      pvi->bmiHeader.biCompression == MAKEFOURCC('U', 'Y', 'V', 'Y')) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_UYVY);
    return true;
  }
  if (sub_type == MEDIASUBTYPE_MJPG &&
      pvi->bmiHeader.biCompression == MAKEFOURCC('M', 'J', 'P', 'G')) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_MJPEG);
    return true;
  }
  if (sub_type == MEDIASUBTYPE_RGB24 &&
      pvi->bmiHeader.biCompression == BI_RGB) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_RGB24);
    return true;
  }
  if (sub_type == MEDIASUBTYPE_RGB32 &&
      pvi->bmiHeader.biCompression == BI_RGB) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_RGB32);
    return true;
  }
  if (sub_type == kMediaSubTypeY16 &&
      pvi->bmiHeader.biCompression == MAKEFOURCC('Y', '1', '6', ' ')) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_Y16);
    return true;
  }
  if (sub_type == kMediaSubTypeZ16 &&
      pvi->bmiHeader.biCompression == MAKEFOURCC('Z', '1', '6', ' ')) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_Y16);
    return true;
  }
  if (sub_type == kMediaSubTypeINVZ &&
      pvi->bmiHeader.biCompression == MAKEFOURCC('I', 'N', 'V', 'Z')) {
    resulting_format_.set_pixel_format(CameraFormat::PIXEL_FORMAT_Y16);
    return true;
  }

#ifndef NDEBUG
  WCHAR guid_str[128];
  StringFromGUID2(sub_type, guid_str, base::size(guid_str));
  DVLOG(2) << __func__ << " unsupported media type: " << guid_str;
#endif
  return false;
}

bool SinkInputPin::GetValidMediaType(int index, AM_MEDIA_TYPE* media_type) {
  if (media_type->cbFormat < sizeof(VIDEOINFOHEADER)) return false;

  VIDEOINFOHEADER* const pvi =
      reinterpret_cast<VIDEOINFOHEADER*>(media_type->pbFormat);

  ZeroMemory(pvi, sizeof(VIDEOINFOHEADER));
  pvi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  pvi->bmiHeader.biPlanes = 1;
  pvi->bmiHeader.biClrImportant = 0;
  pvi->bmiHeader.biClrUsed = 0;
  if (requested_frame_rate_ > 0)
    pvi->AvgTimePerFrame = kSecondsToReferenceTime / requested_frame_rate_;

  media_type->majortype = MEDIATYPE_Video;
  media_type->formattype = FORMAT_VideoInfo;
  media_type->bTemporalCompression = FALSE;

  if (requested_pixel_format_ == CameraFormat::PIXEL_FORMAT_MJPEG ||
      requested_pixel_format_ == CameraFormat::PIXEL_FORMAT_Y16) {
    // If the requested pixel format is MJPEG or Y16, don't accept other.
    // This is ok since the capabilities of the capturer have been
    // enumerated and we know that it is supported.
    if (index != 0) return false;

    pvi->bmiHeader = requested_info_header_;
    return true;
  }

  switch (index) {
    case 0: {
      pvi->bmiHeader.biCompression = MAKEFOURCC('I', '4', '2', '0');
      pvi->bmiHeader.biBitCount = 12;  // bit per pixel
      pvi->bmiHeader.biWidth = requested_info_header_.biWidth;
      pvi->bmiHeader.biHeight = requested_info_header_.biHeight;
      pvi->bmiHeader.biSizeImage = GetArea(requested_info_header_) * 3 / 2;
      media_type->subtype = kMediaSubTypeI420;
      break;
    }
    case 1: {
      pvi->bmiHeader.biCompression = MAKEFOURCC('Y', 'U', 'Y', '2');
      pvi->bmiHeader.biBitCount = 16;
      pvi->bmiHeader.biWidth = requested_info_header_.biWidth;
      pvi->bmiHeader.biHeight = requested_info_header_.biHeight;
      pvi->bmiHeader.biSizeImage = GetArea(requested_info_header_) * 2;
      media_type->subtype = MEDIASUBTYPE_YUY2;
      break;
    }
    case 2: {
      pvi->bmiHeader.biCompression = MAKEFOURCC('U', 'Y', 'V', 'Y');
      pvi->bmiHeader.biBitCount = 16;
      pvi->bmiHeader.biWidth = requested_info_header_.biWidth;
      pvi->bmiHeader.biHeight = requested_info_header_.biHeight;
      pvi->bmiHeader.biSizeImage = GetArea(requested_info_header_) * 2;
      media_type->subtype = MEDIASUBTYPE_UYVY;
      break;
    }
    case 3: {
      pvi->bmiHeader.biCompression = BI_RGB;
      pvi->bmiHeader.biBitCount = 24;
      pvi->bmiHeader.biWidth = requested_info_header_.biWidth;
      pvi->bmiHeader.biHeight = requested_info_header_.biHeight;
      pvi->bmiHeader.biSizeImage = GetArea(requested_info_header_) * 3;
      media_type->subtype = MEDIASUBTYPE_RGB24;
      break;
    }
    case 4: {
      pvi->bmiHeader.biCompression = BI_RGB;
      pvi->bmiHeader.biBitCount = 32;
      pvi->bmiHeader.biWidth = requested_info_header_.biWidth;
      pvi->bmiHeader.biHeight = requested_info_header_.biHeight;
      pvi->bmiHeader.biSizeImage = GetArea(requested_info_header_) * 4;
      media_type->subtype = MEDIASUBTYPE_RGB32;
      break;
    }
    default:
      return false;
  }

  media_type->bFixedSizeSamples = TRUE;
  media_type->lSampleSize = pvi->bmiHeader.biSizeImage;
  return true;
}

HRESULT SinkInputPin::Receive(IMediaSample* sample) {
  const int length = sample->GetActualDataLength();

  if (length <= 0 ||
      static_cast<size_t>(length) < AllocationSize(resulting_format_)) {
    DLOG(WARNING) << "Wrong media sample length: " << length;
    observer_->FrameDropped(
        Status(felicia::error::Code::DATA_LOSS,
               ::base::StringPrintf("Unexpected Sample Length(%d)", length)));
    return S_FALSE;
  }

  uint8_t* buffer = nullptr;
  if (FAILED(sample->GetPointer(&buffer))) {
    observer_->FrameDropped(
        Status(felicia::error::Code::DATA_LOSS,
               "Failed to get memory pointer from media sample."));
    return S_FALSE;
  }

  REFERENCE_TIME start_time, end_time;
  base::TimeDelta timestamp = kNoTimestamp;
  if (SUCCEEDED(sample->GetTime(&start_time, &end_time))) {
    DCHECK(start_time <= end_time);
    timestamp = base::TimeDelta::FromMicroseconds(start_time / 10);
  }

  observer_->FrameReceived(buffer, length, resulting_format_, timestamp);
  return S_OK;
}

SinkInputPin::~SinkInputPin() {}

}  // namespace felicia
