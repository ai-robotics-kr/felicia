#ifndef FELICIA_PYTHON_COMMUNICATION_PY_HELPER_H_
#define FELICIA_PYTHON_COMMUNICATION_PY_HELPER_H_

#include "pybind11/pybind11.h"

#include "felicia/core/communication/publisher.h"
#include "felicia/core/communication/subscriber.h"
#include "felicia/python/type_conversion/callback.h"
#include "felicia/python/type_conversion/protobuf.h"

SUPPORT_PROTOBUF_TYPE_CAST(::felicia::ChannelDef, ChannelDef,
                           felicia.core.protobuf.channel_pb2)
SUPPORT_PROTOBUF_TYPE_CAST(::felicia::NodeInfo, NodeInfo,
                           felicia.core.protobuf.master_data_pb2)

namespace py = pybind11;

namespace felicia {

template <typename MessageTy>
using PyMessageCallback = PyCallback<void(MessageTy&&)>;

template <typename MessageTy>
void AddPublisher(py::module& m, const char* name) {
  py::class_<Publisher<MessageTy>>(m, name)
      .def(py::init<>())
      .def("is_registering", &Publisher<MessageTy>::IsRegistering)
      .def("is_registered", &Publisher<MessageTy>::IsRegistered)
      .def("is_unregistering", &Publisher<MessageTy>::IsUnregistering)
      .def("is_unregistered", &Publisher<MessageTy>::IsUnregistered)
      .def("request_publish",
           [](Publisher<MessageTy>& self, const NodeInfo& node_info,
              const std::string& topic, const ChannelDef& channel_def,
              const communication::Settings& settings, py::function callback) {
             callback.inc_ref();
             return self.RequestPublish(
                 node_info, topic, channel_def, settings,
                 ::base::BindOnce(
                     &PyStatusOnceCallback::Invoke,
                     ::base::Owned(new PyStatusOnceCallback(callback))));
           })
      .def("publish",
           [](Publisher<MessageTy>& self, const MessageTy& message,
              py::function callback) {
             callback.inc_ref();
             return self.Publish(
                 message,
                 ::base::BindOnce(
                     &PyStatusOnceCallback::Invoke,
                     ::base::Owned(new PyStatusOnceCallback(callback))));
           })
      .def("request_unpublish",
           [](Publisher<MessageTy>& self, const NodeInfo& node_info,
              const std::string& topic, py::function callback) {
             callback.inc_ref();
             return self.RequestUnpublish(
                 node_info, topic,
                 ::base::BindOnce(
                     &PyStatusOnceCallback::Invoke,
                     ::base::Owned(new PyStatusOnceCallback(callback))));
           });
}

template <typename MessageTy>
void AddSubscriber(py::module& m, const char* name) {
  py::class_<Subscriber<MessageTy>>(m, name)
      .def(py::init<>())
      .def("is_registering", &Subscriber<MessageTy>::IsRegistering)
      .def("is_registered", &Subscriber<MessageTy>::IsRegistered)
      .def("is_unregistering", &Subscriber<MessageTy>::IsUnregistering)
      .def("is_unregistered", &Subscriber<MessageTy>::IsUnregistered)
      .def("is_started", &Subscriber<MessageTy>::IsStarted)
      .def("is_stopped", &Subscriber<MessageTy>::IsStopped)
      .def(
          "request_subscribe",
          [](Subscriber<MessageTy>& self, const NodeInfo& node_info,
             const std::string& topic, py::function on_message_callback,
             py::function on_error_callback,
             const communication::Settings& settings, py::function callback) {
            on_message_callback.inc_ref();
            on_error_callback.inc_ref();
            callback.inc_ref();
            return self.RequestSubscribe(
                node_info, topic,
                ::base::BindRepeating(
                    &PyMessageCallback<MessageTy>::Invoke,
                    ::base::Owned(
                        new PyMessageCallback<MessageTy>(on_message_callback))),
                ::base::BindRepeating(
                    &PyStatusOnceCallback::Invoke,
                    ::base::Owned(new PyStatusOnceCallback(on_error_callback))),
                settings,
                ::base::BindOnce(
                    &PyStatusOnceCallback::Invoke,
                    ::base::Owned(new PyStatusOnceCallback(callback))));
          })
      .def("request_unsubscribe",
           [](Subscriber<MessageTy>& self, const NodeInfo& node_info,
              const std::string& topic, py::function callback) {
             callback.inc_ref();
             return self.RequestUnsubscribe(
                 node_info, topic,
                 ::base::BindOnce(
                     &PyStatusOnceCallback::Invoke,
                     ::base::Owned(new PyStatusOnceCallback(callback))));
           });
}

}  // namespace felicia

#endif  // FELICIA_PYTHON_COMMUNICATION_PY_HELPER_H_