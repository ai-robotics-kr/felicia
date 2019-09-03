#include "felicia/examples/learn/message_communication/protobuf/cc/simple_subscribing_node.h"

#include "felicia/core/master/master_proxy.h"

namespace felicia {

SimpleSubscribingNode::SimpleSubscribingNode(
    const NodeCreateFlag& node_create_flag)
    : node_create_flag_(node_create_flag),
      topic_(node_create_flag_.topic_flag()->value()) {}

void SimpleSubscribingNode::OnInit() {
  std::cout << "SimpleSubscribingNode::OnInit()" << std::endl;
}

void SimpleSubscribingNode::OnDidCreate(const NodeInfo& node_info) {
  std::cout << "SimpleSubscribingNode::OnDidCreate()" << std::endl;
  node_info_ = node_info;
  RequestSubscribe();

  // MasterProxy& master_proxy = MasterProxy::GetInstance();
  // master_proxy.PostDelayedTask(
  //     FROM_HERE,
  //     base::BindOnce(&SimpleSubscribingNode::RequestUnsubscribe,
  //                      base::Unretained(this)),
  //     base::TimeDelta::FromSeconds(10));
}

void SimpleSubscribingNode::OnError(const Status& s) {
  std::cout << "SimpleSubscribingNode::OnError()" << std::endl;
  LOG(ERROR) << s;
}

void SimpleSubscribingNode::OnMessage(MessageSpec&& message) {
  std::cout << "SimpleSubscribingNode::OnMessage()" << std::endl;
  std::cout << "message : " << message.DebugString() << std::endl;
}

void SimpleSubscribingNode::OnMessageError(const Status& s) {
  std::cout << "SimpleSubscribingNode::OnMessageError()" << std::endl;
  LOG(ERROR) << s;
}

void SimpleSubscribingNode::OnRequestSubscribe(const Status& s) {
  std::cout << "SimpleSubscribingNode::OnRequestSubscribe()" << std::endl;
  LOG_IF(ERROR, !s.ok()) << s;
}

void SimpleSubscribingNode::OnRequestUnsubscribe(const Status& s) {
  std::cout << "SimpleSubscribingNode::OnRequestUnsubscribe()" << std::endl;
  LOG_IF(ERROR, !s.ok()) << s;
}

void SimpleSubscribingNode::RequestSubscribe() {
  communication::Settings settings;
  settings.buffer_size = Bytes::FromBytes(512);
  settings.channel_settings.tcp_settings.use_ssl =
      node_create_flag_.use_ssl_flag()->value();

  subscriber_.RequestSubscribe(
      node_info_, topic_,
      ChannelDef::CHANNEL_TYPE_TCP | ChannelDef::CHANNEL_TYPE_UDP |
          ChannelDef::CHANNEL_TYPE_UDS | ChannelDef::CHANNEL_TYPE_SHM,
      settings,
      base::BindRepeating(&SimpleSubscribingNode::OnMessage,
                          base::Unretained(this)),
      base::BindRepeating(&SimpleSubscribingNode::OnMessageError,
                          base::Unretained(this)),
      base::BindOnce(&SimpleSubscribingNode::OnRequestSubscribe,
                     base::Unretained(this)));
}

void SimpleSubscribingNode::RequestUnsubscribe() {
  subscriber_.RequestUnsubscribe(
      node_info_, topic_,
      base::BindOnce(&SimpleSubscribingNode::OnRequestUnsubscribe,
                     base::Unretained(this)));
}

}  // namespace felicia