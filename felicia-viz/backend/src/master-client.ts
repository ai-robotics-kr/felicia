// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import {
  ListClientsRequestProtobuf,
  ListClientsResponseProtobuf,
  ListNodesRequestProtobuf,
  ListNodesResponseProtobuf,
  ListServicesRequestProtobuf,
  ListServicesResponseProtobuf,
  ListTopicsRequestProtobuf,
  ListTopicsResponseProtobuf,
  PublishTopicRequestProtobuf,
  PublishTopicResponseProtobuf,
  RegisterClientRequestProtobuf,
  RegisterClientResponseProtobuf,
  RegisterNodeRequestProtobuf,
  RegisterNodeResponseProtobuf,
  RegisterServiceClientRequestProtobuf,
  RegisterServiceClientResponseProtobuf,
  RegisterServiceServerRequestProtobuf,
  RegisterServiceServerResponseProtobuf,
  SubscribeTopicRequestProtobuf,
  SubscribeTopicResponseProtobuf,
  UnpublishTopicRequestProtobuf,
  UnpublishTopicResponseProtobuf,
  UnregisterNodeRequestProtobuf,
  UnregisterNodeResponseProtobuf,
  UnregisterServiceClientRequestProtobuf,
  UnregisterServiceClientResponseProtobuf,
  UnregisterServiceServerRequestProtobuf,
  UnregisterServiceServerResponseProtobuf,
  UnsubscribeTopicRequestProtobuf,
  UnsubscribeTopicResponseProtobuf,
} from '@felicia-viz/proto/messages/master';
import { loadSync } from '@grpc/proto-loader';
import grpc, { Client, status, ServiceError } from 'grpc';
import path from 'path';
import { FELICIA_ROOT } from 'typings/settings';

type Callback = (err: Error | null, response: string | null) => void;

interface MasterClientInterface extends Client {
  RegisterClient: (
    request: RegisterClientRequestProtobuf,
    callback: (err: Error | null, response: RegisterClientResponseProtobuf) => void
  ) => void;
  ListClients: (
    request: ListClientsRequestProtobuf,
    callback: (err: Error | null, response: ListClientsResponseProtobuf) => void
  ) => void;
  RegisterNode: (
    request: RegisterNodeRequestProtobuf,
    callback: (err: Error | null, response: RegisterNodeResponseProtobuf) => void
  ) => void;
  UnregisterNode: (
    request: UnregisterNodeRequestProtobuf,
    callback: (err: Error | null, response: UnregisterNodeResponseProtobuf) => void
  ) => void;
  ListNodes: (
    request: ListNodesRequestProtobuf,
    callback: (err: Error | null, response: ListNodesResponseProtobuf) => void
  ) => void;
  PublishTopic: (
    request: PublishTopicRequestProtobuf,
    callback: (err: Error | null, response: PublishTopicResponseProtobuf) => void
  ) => void;
  UnpublishTopic: (
    request: UnpublishTopicRequestProtobuf,
    callback: (err: Error | null, response: UnpublishTopicResponseProtobuf) => void
  ) => void;
  SubscribeTopic: (
    request: SubscribeTopicRequestProtobuf,
    callback: (err: Error | null, response: SubscribeTopicResponseProtobuf) => void
  ) => void;
  UnsubscribeTopic: (
    request: UnsubscribeTopicRequestProtobuf,
    callback: (err: Error | null, response: UnsubscribeTopicResponseProtobuf) => void
  ) => void;
  ListTopics: (
    request: ListTopicsRequestProtobuf,
    callback: (err: Error | null, response: ListTopicsResponseProtobuf) => void
  ) => void;
  RegisterServiceClient: (
    request: RegisterServiceClientRequestProtobuf,
    callback: (err: Error | null, response: RegisterServiceClientResponseProtobuf) => void
  ) => void;
  UnregisterServiceClient: (
    request: UnregisterServiceClientRequestProtobuf,
    callback: (err: Error | null, response: UnregisterServiceClientResponseProtobuf) => void
  ) => void;
  RegisterServiceServer: (
    request: RegisterServiceServerRequestProtobuf,
    callback: (err: Error | null, response: RegisterServiceServerResponseProtobuf) => void
  ) => void;
  UnregisterServiceServer: (
    request: UnregisterServiceServerRequestProtobuf,
    callback: (err: Error | null, response: UnregisterServiceServerResponseProtobuf) => void
  ) => void;
  ListServices: (
    request: ListServicesRequestProtobuf,
    callback: (err: Error | null, response: ListServicesResponseProtobuf) => void
  ) => void;
}

type MasterClientMethodName =
  | 'RegisterClient'
  | 'ListClients'
  | 'RegisterNode'
  | 'UnregisterNode'
  | 'ListNodes'
  | 'PublishTopic'
  | 'UnpublishTopic'
  | 'SubscribeTopic'
  | 'UnsubscribeTopic'
  | 'ListTopics'
  | 'RegisterServiceClient'
  | 'UnregisterServiceClient'
  | 'RegisterServiceServer'
  | 'UnregisterServiceServer'
  | 'ListServices';

export default class MasterClient {
  client: MasterClientInterface | null = null;

  start(address: string) {
    const packageDefinition = loadSync(
      path.resolve(FELICIA_ROOT, 'felicia/core/master/rpc/master_service.proto'),
      {
        longs: String,
        enums: String,
        defaults: true,
        oneofs: true,
        includeDirs: [FELICIA_ROOT],
      }
    );
    const feliciaProto = grpc.loadPackageDefinition(packageDefinition).felicia as any;
    this.client = new (feliciaProto.MasterService as typeof Client)(
      address,
      grpc.credentials.createInsecure()
    ) as MasterClientInterface;
  }

  stop() {
    if (this.client) {
      this.client.close();
      this.client = null;
    }
  }

  _callGrpc(method: MasterClientMethodName, request: any, callback: Callback): void {
    if (this.client) {
      this.client[method](
        request,
        (err: Error | null, response: any): void => {
          if (err) {
            callback(err, null);
            return;
          }
          callback(null, JSON.stringify(response));
        }
      );
    } else {
      const err: ServiceError = new Error();
      err.code = status.ABORTED;
      err.details = 'client is closed';
      callback(err, null);
    }
  }

  registerClient(request: RegisterClientRequestProtobuf, callback: Callback): void {
    this._callGrpc('RegisterClient', request, callback);
  }

  listClients(request: ListClientsRequestProtobuf, callback: Callback): void {
    this._callGrpc('ListClients', request, callback);
  }

  registerNode(request: RegisterNodeRequestProtobuf, callback: Callback): void {
    this._callGrpc('RegisterNode', request, callback);
  }

  unregisterNode(request: UnregisterNodeRequestProtobuf, callback: Callback): void {
    this._callGrpc('UnregisterNode', request, callback);
  }

  listNodes(request: ListNodesRequestProtobuf, callback: Callback): void {
    this._callGrpc('ListNodes', request, callback);
  }

  publishTopic(request: PublishTopicRequestProtobuf, callback: Callback): void {
    this._callGrpc('PublishTopic', request, callback);
  }

  unpublishTopic(request: UnpublishTopicRequestProtobuf, callback: Callback): void {
    this._callGrpc('UnpublishTopic', request, callback);
  }

  subscribeTopic(request: SubscribeTopicRequestProtobuf, callback: Callback): void {
    this._callGrpc('SubscribeTopic', request, callback);
  }

  unsubscribeTopic(request: UnsubscribeTopicRequestProtobuf, callback: Callback): void {
    this._callGrpc('UnsubscribeTopic', request, callback);
  }

  listTopics(request: ListTopicsRequestProtobuf, callback: Callback): void {
    this._callGrpc('ListTopics', request, callback);
  }

  registerServiceClient(request: RegisterServiceClientRequestProtobuf, callback: Callback): void {
    this._callGrpc('RegisterServiceClient', request, callback);
  }

  unregisterServiceClient(
    request: UnregisterServiceClientRequestProtobuf,
    callback: Callback
  ): void {
    this._callGrpc('UnregisterServiceClient', request, callback);
  }

  registerServiceServer(request: RegisterServiceServerRequestProtobuf, callback: Callback): void {
    this._callGrpc('RegisterServiceServer', request, callback);
  }

  unregisterServiceServer(
    request: UnregisterServiceServerRequestProtobuf,
    callback: Callback
  ): void {
    this._callGrpc('UnregisterServiceServer', request, callback);
  }

  listServices(request: ListServicesRequestProtobuf, callback: Callback): void {
    this._callGrpc('ListServices', request, callback);
  }
}
