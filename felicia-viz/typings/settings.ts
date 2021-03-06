// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

declare const FELICIA_ROOT: string;
declare const HEARTBEAT_INTERVAL: number;
declare const HTTP_PORT: number;
declare const SERVER_ADDRESS: string;
declare const WEBSOCKET_PORT: number;

const _FELICIA_ROOT = FELICIA_ROOT;
const _HEARTBEAT_INTERVAL = HEARTBEAT_INTERVAL;
const _HTTP_PORT = HTTP_PORT;
const _WEBSOCKET_PORT = WEBSOCKET_PORT;
const _SERVER_ADDRESS = SERVER_ADDRESS;

export {
  _FELICIA_ROOT as FELICIA_ROOT,
  _HEARTBEAT_INTERVAL as HEARTBEAT_INTERVAL,
  _HTTP_PORT as HTTP_PORT,
  _SERVER_ADDRESS as SERVER_ADDRESS,
  _WEBSOCKET_PORT as WEBSOCKET_PORT,
};
