// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_CORE_NODE_TOPIC_INFO_WATCHER_NODE_H_
#define FELICIA_CORE_NODE_TOPIC_INFO_WATCHER_NODE_H_

#include "felicia/core/lib/base/export.h"
#include "felicia/core/master/master_notification_watcher.h"
#include "felicia/core/node/node_lifecycle.h"

namespace felicia {

class FEL_EXPORT TopicInfoWatcherNode : public NodeLifecycle {
 public:
  class FEL_EXPORT Delegate {
   public:
    virtual ~Delegate() = default;

    virtual void OnNewTopicInfo(const TopicInfo& topic_info) = 0;
    virtual void OnError(Status s) = 0;
  };

  TopicInfoWatcherNode(std::unique_ptr<Delegate> delegate);

  ~TopicInfoWatcherNode();

  void OnInit() override;

  void OnDidCreate(NodeInfo node_info) override;

  void OnError(Status s) override;

 private:
  std::unique_ptr<Delegate> delegate_;
  NodeInfo node_info_;
};

}  // namespace felicia

#endif  // FELICIA_CORE_NODE_TOPIC_INFO_WATCHER_NODE_H_