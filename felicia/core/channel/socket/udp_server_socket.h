// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_CORE_CHANNEL_SOCKET_UDP_SERVER_SOCKET_H_
#define FELICIA_CORE_CHANNEL_SOCKET_UDP_SERVER_SOCKET_H_

#include "felicia/core/channel/socket/udp_socket.h"

#include "felicia/core/lib/error/statusor.h"
#include "felicia/core/protobuf/channel.pb.h"

namespace felicia {

class UDPServerSocket : public UDPSocket {
 public:
  UDPServerSocket();
  ~UDPServerSocket();

  bool IsServer() const override;

  StatusOr<ChannelDef> Bind();

  // ChannelImpl methods
  void WriteAsync(scoped_refptr<net::IOBuffer> buffer, int size,
                  StatusOnceCallback callback) override;
  void ReadAsync(scoped_refptr<net::GrowableIOBuffer> buffer, int size,
                 StatusOnceCallback callback) override;

  DISALLOW_COPY_AND_ASSIGN(UDPServerSocket);
};

}  // namespace felicia

#endif  // FELICIA_CORE_CHANNEL_SOCKET_UDP_SERVER_SOCKET_H_