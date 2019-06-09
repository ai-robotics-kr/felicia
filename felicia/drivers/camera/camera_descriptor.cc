#include "felicia/drivers/camera/camera_descriptor.h"

#include "third_party/chromium/base/strings/strcat.h"

namespace felicia {

CameraDescriptor::CameraDescriptor() = default;

CameraDescriptor::CameraDescriptor(const std::string& display_name,
                                   const std::string& device_id,
                                   const std::string& model_id)
    : display_name_(display_name), device_id_(device_id), model_id_(model_id) {}

CameraDescriptor::~CameraDescriptor() = default;

CameraDescriptor::CameraDescriptor(const CameraDescriptor& other) = default;
CameraDescriptor& CameraDescriptor::operator=(const CameraDescriptor& other) =
    default;

const std::string& CameraDescriptor::display_name() const {
  return display_name_;
}

const std::string& CameraDescriptor::device_id() const { return device_id_; }

const std::string& CameraDescriptor::model_id() const { return model_id_; }

std::string CameraDescriptor::ToString() const {
  return ::base::StrCat({"display_name: ", display_name_,
                         " device_id: ", device_id_, " model_id: ", model_id_});
}

std::ostream& operator<<(std::ostream& os,
                         const CameraDescriptor& camera_descriptor) {
  os << camera_descriptor.ToString();
  return os;
}

std::ostream& operator<<(std::ostream& os,
                         const CameraDescriptors& camera_descriptors) {
  for (size_t i = 0; i < camera_descriptors.size(); ++i) {
    os << "[" << i << "] " << camera_descriptors[i] << std::endl;
  }
  return os;
}

}  // namespace felicia