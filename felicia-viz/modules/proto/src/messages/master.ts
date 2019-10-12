import {
  ClientFilterProtobuf,
  ClientInfoProtobuf,
  NodeInfoProtobuf,
  PubSubTopicsProtobuf,
  TopicFilterProtobuf,
  TopicInfoProtobuf,
} from './master-data';

export interface RegisterClientRequestProtobuf {
  clientInfo: ClientInfoProtobuf;
}

export interface RegisterClientResponseProtobuf {
  id: number;
}

export interface ListClientsRequestProtobuf {
  clientFilter: ClientFilterProtobuf;
}

export interface ListClientsResponseProtobuf {
  clientInfos: Array<ClientInfoProtobuf>;
}

export interface RegisterNodeRequestProtobuf {
  nodeInfo: NodeInfoProtobuf;
}

export interface RegisterNodeResponseProtobuf {
  nodeInfo: NodeInfoProtobuf;
}

export interface UnregisterNodeRequestProtobuf {
  nodeInfo: NodeInfoProtobuf;
}

export interface UnregisterNodeResponseProtobuf {}

export interface ListNodesRequestProtobuf {
  nodeFilter: NodeFilter;
}

export interface ListNodesResponseProtobuf {
  nodeInfos: Array<NodeInfoProtobuf>;
  pubSubTopics: PubSubTopicsProtobuf;
}

export interface PublishTopicRequestProtobuf {
  nodeInfo: NodeInfoProtobuf;
  topicInfo: TopicInfoProtobuf;
}

export interface PublishTopicResponseProtobuf {}

export interface UnpublishTopicRequestProtobuf {
  nodeInfo: NodeInfoProtobuf;
  topic: string;
}

export interface UnpublishTopicResponseProtobuf {}

export interface SubscribeTopicRequestProtobuf {
  nodeInfo: NodeInfoProtobuf;
  topic: string;
  topicType: string;
}

export interface SubscribeTopicResponseProtobuf {}

export interface UnsubscribeTopicRequestProtobuf {
  nodeInfo: NodeInfoProtobuf;
  topic: string;
}

export interface UnsubscribeTopicResponseProtobuf {}

export interface ListTopicsRequestProtobuf {
  topicFilter: TopicFilterProtobuf;
}

export interface ListTopicsResponseProtobuf {
  topicInfos: Array<TopicInfoProtobuf>;
}