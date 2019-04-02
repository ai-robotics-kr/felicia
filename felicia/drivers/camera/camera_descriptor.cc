#include "felicia/drivers/camera/camera_descriptor.h"

namespace felicia {

CameraDescriptor::CameraDescriptor(const std::string& display_name,
                                   const std::string& device_id)
    : display_name_(display_name), device_id_(device_id) {}

CameraDescriptor::~CameraDescriptor() = default;

CameraDescriptor::CameraDescriptor(const CameraDescriptor& other) = default;
CameraDescriptor& CameraDescriptor::operator=(const CameraDescriptor& other) =
    default;

const std::string& CameraDescriptor::display_name() const {
  return display_name_;
}

const std::string& CameraDescriptor::device_id() const { return device_id_; }

}  // namespace felicia