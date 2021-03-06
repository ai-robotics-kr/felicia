// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_CORE_MASTER_RPC_MASTER_CLIENT_H_
#define FELICIA_CORE_MASTER_RPC_MASTER_CLIENT_H_

#include "felicia/core/lib/base/export.h"
#include "felicia/core/master/master_client_interface.h"
#include "felicia/core/master/rpc/master_service.grpc.pb.h"
#include "felicia/core/rpc/client.h"

namespace felicia {

class MasterClient : public rpc::Client<grpc::MasterService>,
                     public MasterClientInterface {
 public:
  explicit MasterClient(std::shared_ptr<::grpc::Channel> channel);
  ~MasterClient();

  Status Start() override;
  Status Stop() override;

#define MASTER_METHOD(Method, method, cancelable) \
  FEL_GRPC_CLIENT_METHOD_DECLARE(Method) override;
#include "felicia/core/master/rpc/master_method_list.h"
#undef MASTER_METHOD
};

}  // namespace felicia

#endif  // FELICIA_CORE_MASTER_RPC_MASTER_CLIENT_H_