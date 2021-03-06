// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELIICA_CORE_RPC_GRPC_SERVER_IMPL_H_
#define FELIICA_CORE_RPC_GRPC_SERVER_IMPL_H_

#include <memory>

#include "grpcpp/grpcpp.h"
#include "third_party/chromium/base/bind.h"
#include "third_party/chromium/base/callback.h"
#include "third_party/chromium/base/macros.h"
#include "third_party/chromium/base/strings/stringprintf.h"
#include "third_party/chromium/base/threading/thread.h"

#include "felicia/core/lib/error/errors.h"
#include "felicia/core/rpc/grpc_util.h"
#include "felicia/core/rpc/server_interface.h"

namespace felicia {
namespace rpc {

#define FEL_GRPC_SERVER \
  Server<T, std::enable_if_t<IsGrpcServiceWrapper<T>::value>>

template <typename T>
class FEL_GRPC_SERVER : public ServerInterface {
 public:
  typedef T Service;

  Server() = default;
  ~Server() override = default;

  Status Start() override;

  // Non-blocking
  Status Run() override {
    RunRpcsLoops(1);
    return Status::OK();
  }

  Status Shutdown() override {
    ShutdownServer();
    return Status::OK();
  }

  std::string GetServiceTypeName() const override {
    return Service::service_name();
  }

  void WaitUntilShutdown() { threads_.clear(); }

 protected:
  virtual Status RegisterService(::grpc::ServerBuilder* builder) = 0;

  void RunRpcsLoops(int num_threads);
  void ShutdownServer();

  std::unique_ptr<Service> service_;
  std::unique_ptr<::grpc::Server> server_;
  std::vector<std::unique_ptr<base::Thread>> threads_;

  DISALLOW_COPY_AND_ASSIGN(Server);
};

template <typename T>
Status FEL_GRPC_SERVER::Start() {
  ::grpc::ServerBuilder builder;
  builder.AddListeningPort(ConfigureServerAddress(),
                           ::grpc::InsecureServerCredentials());
  builder.SetMaxMessageSize(std::numeric_limits<int32_t>::max());
  Status s = RegisterService(&builder);
  if (!s.ok()) return s;
  server_ = builder.BuildAndStart();

  if (!server_) {
    return errors::Unknown("Could not start gRPC server");
  }

  return Status::OK();
}

template <typename T>
void FEL_GRPC_SERVER::RunRpcsLoops(int num_threads) {
  for (int i = 0; i < num_threads; ++i) {
    threads_.push_back(std::make_unique<base::Thread>(base::StringPrintf(
        "%s RPC Loop%d", Service::service_name().c_str(), (i + 1))));
  }

  std::for_each(threads_.begin(), threads_.end(),
                [this](const std::unique_ptr<base::Thread>& thread) {
                  thread->Start();
                  thread->task_runner()->PostTask(
                      FROM_HERE,
                      base::BindOnce(&Service::HandleRpcsLoop,
                                     base::Unretained(service_.get())));
                });
}

template <typename T>
void FEL_GRPC_SERVER::ShutdownServer() {
  if (server_) server_->Shutdown();
  if (service_) service_->Shutdown();
}

}  // namespace rpc
}  // namespace felicia

#endif  // FELIICA_CORE_RPC_GRPC_SERVER_IMPL_H_