// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "felicia/core/master/tool/topic_publish_command_dispatcher.h"

#include "felicia/core/master/master_proxy.h"
#include "felicia/core/node/dynamic_publishing_node.h"
#include "felicia/core/thread/main_thread.h"

namespace felicia {

namespace {

class DynamicPublisherDelegate : public DynamicPublishingNode::Delegate {
 public:
  DynamicPublisherDelegate(const std::string& message_type,
                           const std::string& topic,
                           const std::string& channel_type,
                           const std::string& message, int64_t interval)
      : message_type_(message_type),
        topic_(topic),
        message_(message),
        delay_(base::TimeDelta::FromMilliseconds(interval)) {
    ChannelDef::Type_Parse(channel_type, &channel_type_);
  }

  void OnDidCreate(DynamicPublishingNode* node) override {
    node_ = node;

    communication::Settings settings;
    settings.is_dynamic_buffer = true;

    node_->RequestPublish(message_type_, topic_, channel_type_, settings);
  }

  void OnError(Status s) override { NOTREACHED() << s; }

  void OnRequestPublish(Status s) override {
    CHECK(s.ok()) << s;
    PublishMessageFromJson();
  }

  void PublishMessageFromJson() {
    node_->PublishMessageFromJson(message_);
    MainThread& main_thread = MainThread::GetInstance();
    main_thread.PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&DynamicPublisherDelegate::PublishMessageFromJson,
                       base::Unretained(this)),
        delay_);
  }

 private:
  std::string message_type_;
  std::string topic_;
  ChannelDef::Type channel_type_;
  std::string message_;
  base::TimeDelta delay_;
  DynamicPublishingNode* node_;
};

}  // namespace

TopicPublishCommandDispatcher::TopicPublishCommandDispatcher() = default;

void TopicPublishCommandDispatcher::Dispatch(
    const TopicPublishFlag& delegate) const {
  MasterProxy& master_proxy = MasterProxy::GetInstance();

  auto dynamic_publisher_delegate = std::make_unique<DynamicPublisherDelegate>(
      delegate.message_type_flag()->value(), delegate.topic_flag()->value(),
      delegate.channel_type_flag()->value(), delegate.message_flag()->value(),
      delegate.interval_flag()->value());

  NodeInfo node_info;

  master_proxy.RequestRegisterNode<DynamicPublishingNode>(
      node_info, std::move(dynamic_publisher_delegate));
}

}  // namespace felicia