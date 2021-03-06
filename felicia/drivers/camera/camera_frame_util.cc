// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Followings are taken and modified from chromium/media/base/video_frames.cc
// - bool RequiresEvenSizeAllocation(CameraFormat camera_format);
// - size_t AllocationSize(CameraFormat camera_format);
// - bool IsValidPlane(size_t plane, CameraFormat camera_format);
// - Sizei SampleSize(CameraFormat camera_format, size_t plane);
// - int BytesPerElement(CameraFormat camera_format, size_t plane);
// - Sizei PlaneSize(CameraFormat camera_format, size_t plane);
// Followings are taken and modified from
// chromium/media/base/video_frame_layout.cc
// - size_t NumPlanes(CameraFormat camera_format);

#include "felicia/drivers/camera/camera_frame_util.h"

#include "third_party/chromium/base/bits.h"
#include "third_party/chromium/base/logging.h"
#include "third_party/chromium/base/stl_util.h"

namespace felicia {
namespace drivers {
namespace camera_internal {

namespace {

enum {
  kMaxPlanes = 4,

  kYPlane = 0,
  kARGBPlane = kYPlane,
  kUPlane = 1,
  kUVPlane = kUPlane,
  kVPlane = 2,
  kAPlane = 3,
};

// If it is required to allocate aligned to multiple-of-two size overall for the
// frame of pixel |camera_format|.
bool RequiresEvenSizeAllocation(const CameraFormat& camera_format) {
  PixelFormat pixel_format = camera_format.pixel_format();
  switch (pixel_format) {
    case PIXEL_FORMAT_BGRA:
    case PIXEL_FORMAT_BGR:
    case PIXEL_FORMAT_BGRX:
    case PIXEL_FORMAT_Y8:
    case PIXEL_FORMAT_Y16:
    case PIXEL_FORMAT_RGBA:
    case PIXEL_FORMAT_RGBX:
    case PIXEL_FORMAT_RGB:
    case PIXEL_FORMAT_ARGB:
    case PIXEL_FORMAT_Z16:
      return false;
    case PIXEL_FORMAT_NV12:
    case PIXEL_FORMAT_NV21:
    // case PIXEL_FORMAT_MT21:
    case PIXEL_FORMAT_I420:
    case PIXEL_FORMAT_MJPEG:
    case PIXEL_FORMAT_YUY2:
    case PIXEL_FORMAT_YV12:
    // case PIXEL_FORMAT_I422:
    // case PIXEL_FORMAT_I444:
    // case PIXEL_FORMAT_YUV420P9:
    // case PIXEL_FORMAT_YUV422P9:
    // case PIXEL_FORMAT_YUV444P9:
    // case PIXEL_FORMAT_YUV420P10:
    // case PIXEL_FORMAT_YUV422P10:
    // case PIXEL_FORMAT_YUV444P10:
    // case PIXEL_FORMAT_YUV420P12:
    // case PIXEL_FORMAT_YUV422P12:
    // case PIXEL_FORMAT_YUV444P12:
    // case PIXEL_FORMAT_I420A:
    case PIXEL_FORMAT_UYVY:
      // case PIXEL_FORMAT_P016LE:
      return true;
    case PIXEL_FORMAT_UNKNOWN:
    case PixelFormat_INT_MIN_SENTINEL_DO_NOT_USE_:
    case PixelFormat_INT_MAX_SENTINEL_DO_NOT_USE_:
      break;
  }
  NOTREACHED() << "Unsupported video frame format: " << pixel_format;
  return false;
}

// static
size_t NumPlanes(const CameraFormat& camera_format) {
  PixelFormat pixel_format = camera_format.pixel_format();
  switch (pixel_format) {
    case PIXEL_FORMAT_UYVY:
    case PIXEL_FORMAT_YUY2:
    case PIXEL_FORMAT_BGRA:
    case PIXEL_FORMAT_BGR:
    case PIXEL_FORMAT_BGRX:
    case PIXEL_FORMAT_MJPEG:
    case PIXEL_FORMAT_Y8:
    case PIXEL_FORMAT_Y16:
    case PIXEL_FORMAT_Z16:
    case PIXEL_FORMAT_RGBA:
    case PIXEL_FORMAT_RGBX:
    case PIXEL_FORMAT_RGB:
    case PIXEL_FORMAT_ARGB:
      return 1;
    case PIXEL_FORMAT_NV12:
    case PIXEL_FORMAT_NV21:
      // case PIXEL_FORMAT_MT21:
      // case PIXEL_FORMAT_P016LE:
      return 2;
    case PIXEL_FORMAT_I420:
    case PIXEL_FORMAT_YV12:
      // case PIXEL_FORMAT_I422:
      // case PIXEL_FORMAT_I444:
      // case PIXEL_FORMAT_YUV420P9:
      // case PIXEL_FORMAT_YUV422P9:
      // case PIXEL_FORMAT_YUV444P9:
      // case PIXEL_FORMAT_YUV420P10:
      // case PIXEL_FORMAT_YUV422P10:
      // case PIXEL_FORMAT_YUV444P10:
      // case PIXEL_FORMAT_YUV420P12:
      // case PIXEL_FORMAT_YUV422P12:
      // case PIXEL_FORMAT_YUV444P12:
      return 3;
    // case PIXEL_FORMAT_I420A:
    //   return 4;
    case PIXEL_FORMAT_UNKNOWN:
    case PixelFormat_INT_MIN_SENTINEL_DO_NOT_USE_:
    case PixelFormat_INT_MAX_SENTINEL_DO_NOT_USE_:
      // Note: PIXEL_FORMAT_UNKNOWN is used for end-of-stream frame.
      // Set its NumPlanes() to zero to avoid NOTREACHED().
      return 0;
  }
  NOTREACHED() << "Unsupported video frame format: " << pixel_format;
  return 0;
}

bool IsValidPlane(size_t plane, const CameraFormat& camera_format) {
  DCHECK_LE(NumPlanes(camera_format), static_cast<size_t>(kMaxPlanes));
  return (plane < NumPlanes(camera_format));
}

Sizei SampleSize(const CameraFormat& camera_format, size_t plane) {
  DCHECK(IsValidPlane(plane, camera_format));
  PixelFormat pixel_format = camera_format.pixel_format();

  switch (plane) {
    case kYPlane:  // and kARGBPlane:
    case kAPlane:
      return Sizei(1, 1);

    case kUPlane:  // and kUVPlane:
    case kVPlane:
      switch (pixel_format) {
          // case PIXEL_FORMAT_I444:
          // case PIXEL_FORMAT_YUV444P9:
          // case PIXEL_FORMAT_YUV444P10:
          // case PIXEL_FORMAT_YUV444P12:
        case PIXEL_FORMAT_Y8:
        case PIXEL_FORMAT_Y16:
        case PIXEL_FORMAT_Z16:
          return Sizei(1, 1);

          // case PIXEL_FORMAT_I422:
          // case PIXEL_FORMAT_YUV422P9:
          // case PIXEL_FORMAT_YUV422P10:
          // case PIXEL_FORMAT_YUV422P12:
          //   return gfx::Size(2, 1);

        case PIXEL_FORMAT_YV12:
        case PIXEL_FORMAT_I420:
        // case PIXEL_FORMAT_I420A:
        case PIXEL_FORMAT_NV12:
        case PIXEL_FORMAT_NV21:
          // case PIXEL_FORMAT_MT21:
          // case PIXEL_FORMAT_YUV420P9:
          // case PIXEL_FORMAT_YUV420P10:
          // case PIXEL_FORMAT_YUV420P12:
          // case PIXEL_FORMAT_P016LE:
          return Sizei(2, 2);

        case PIXEL_FORMAT_UNKNOWN:
        case PixelFormat_INT_MIN_SENTINEL_DO_NOT_USE_:
        case PixelFormat_INT_MAX_SENTINEL_DO_NOT_USE_:
        case PIXEL_FORMAT_UYVY:
        case PIXEL_FORMAT_YUY2:
        case PIXEL_FORMAT_BGRA:
        case PIXEL_FORMAT_BGR:
        case PIXEL_FORMAT_BGRX:
        case PIXEL_FORMAT_MJPEG:
        case PIXEL_FORMAT_RGBA:
        case PIXEL_FORMAT_RGBX:
        case PIXEL_FORMAT_RGB:
        case PIXEL_FORMAT_ARGB:
          break;
      }
  }
  NOTREACHED();
  return Sizei();
}

int BytesPerElement(const CameraFormat& camera_format, size_t plane) {
  DCHECK(IsValidPlane(plane, camera_format));

  PixelFormat pixel_format = camera_format.pixel_format();
  switch (pixel_format) {
    case PIXEL_FORMAT_BGRA:
    case PIXEL_FORMAT_BGRX:
    case PIXEL_FORMAT_RGBA:
    case PIXEL_FORMAT_RGBX:
    case PIXEL_FORMAT_ARGB:
      return 4;
    case PIXEL_FORMAT_BGR:
    case PIXEL_FORMAT_RGB:
      return 3;
    case PIXEL_FORMAT_Y16:
    case PIXEL_FORMAT_Z16:
    case PIXEL_FORMAT_UYVY:
    case PIXEL_FORMAT_YUY2:
      // case PIXEL_FORMAT_YUV420P9:
      // case PIXEL_FORMAT_YUV422P9:
      // case PIXEL_FORMAT_YUV444P9:
      // case PIXEL_FORMAT_YUV420P10:
      // case PIXEL_FORMAT_YUV422P10:
      // case PIXEL_FORMAT_YUV444P10:
      // case PIXEL_FORMAT_YUV420P12:
      // case PIXEL_FORMAT_YUV422P12:
      // case PIXEL_FORMAT_YUV444P12:
      // case PIXEL_FORMAT_P016LE:
      return 2;
    case PIXEL_FORMAT_NV12:
    case PIXEL_FORMAT_NV21: {
      // case PIXEL_FORMAT_MT21: {
      static const int bytes_per_element[] = {1, 2};
      DCHECK_LT(plane, base::size(bytes_per_element));
      return bytes_per_element[plane];
    }
    case PIXEL_FORMAT_Y8:
    case PIXEL_FORMAT_YV12:
    case PIXEL_FORMAT_I420:
      // case PIXEL_FORMAT_I422:
      // case PIXEL_FORMAT_I420A:
      // case PIXEL_FORMAT_I444:
      return 1;
    case PIXEL_FORMAT_MJPEG:
      return 0;
    case PIXEL_FORMAT_UNKNOWN:
    case PixelFormat_INT_MIN_SENTINEL_DO_NOT_USE_:
    case PixelFormat_INT_MAX_SENTINEL_DO_NOT_USE_:
      break;
  }
  NOTREACHED();
  return 0;
}

Sizei PlaneSize(const CameraFormat& camera_format, size_t plane) {
  DCHECK(IsValidPlane(plane, camera_format));

  int width = camera_format.width();
  int height = camera_format.height();
  if (RequiresEvenSizeAllocation(camera_format)) {
    // Align to multiple-of-two size overall. This ensures that non-subsampled
    // planes can be addressed by pixel with the same scaling as the subsampled
    // planes.
    width = base::bits::Align(width, 2);
    height = base::bits::Align(height, 2);
  }

  const Sizei subsample = SampleSize(camera_format, plane);
  DCHECK(width % subsample.width() == 0);
  DCHECK(height % subsample.height() == 0);
  return Sizei(
      BytesPerElement(camera_format, plane) * width / subsample.width(),
      height / subsample.height());
}

}  // namespace

size_t AllocationSize(const CameraFormat& camera_format) {
  size_t total = 0;
  for (size_t i = 0; i < NumPlanes(camera_format); ++i)
    total += PlaneSize(camera_format, i).area();
  return total;
}

}  // namespace camera_internal
}  // namespace drivers
}  // namespace felicia