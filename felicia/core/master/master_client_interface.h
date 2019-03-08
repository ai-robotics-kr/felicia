#ifndef FELICIA_CORE_MASTER_MASTER_CLIENT_INTERFACE_H_
#define FELICIA_CORE_MASTER_MASTER_CLIENT_INTERFACE_H_

#include <string>
#include <vector>

#include "third_party/chromium/base/callback.h"
#include "third_party/chromium/base/macros.h"

#include "felicia/core/lib/base/export.h"
#include "felicia/core/lib/error/status.h"
#include "felicia/core/master/master.pb.h"

namespace felicia {

class EXPORT MasterClientInterface {
 public:
  MasterClientInterface() = default;
  virtual ~MasterClientInterface() = default;

  virtual Status Start() = 0;
  virtual Status Stop() = 0;
  virtual Status Shutdown() = 0;

#define CLIENT_METHOD(method)                                \
  virtual void method##Async(const method##Request* request, \
                             method##Response* response,     \
                             StatusCallback done) = 0

  CLIENT_METHOD(RegisterClient);
  CLIENT_METHOD(ListClients);
  CLIENT_METHOD(RegisterNode);
  CLIENT_METHOD(UnregisterNode);
  CLIENT_METHOD(ListNodes);
  CLIENT_METHOD(PublishTopic);
  CLIENT_METHOD(UnpublishTopic);
  CLIENT_METHOD(SubscribeTopic);
  CLIENT_METHOD(UnsubscribeTopic);
  CLIENT_METHOD(ListTopics);

#undef CLIENT_METHOD

  DISALLOW_COPY_AND_ASSIGN(MasterClientInterface);
};

}  // namespace felicia

#endif  // FELICIA_CORE_MASTER_SERVICE_INTERFACE_H_