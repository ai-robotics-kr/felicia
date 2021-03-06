// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_CORE_CHANNEL_SOCKET_TCP_SERVER_SOCKET_H_
#define FELICIA_CORE_CHANNEL_SOCKET_TCP_SERVER_SOCKET_H_

#include "felicia/core/channel/socket/ssl_server_socket.h"
#include "felicia/core/channel/socket/stream_socket_broadcaster.h"
#include "felicia/core/channel/socket/tcp_client_socket.h"
#include "felicia/core/lib/error/statusor.h"
#include "felicia/core/protobuf/channel.pb.h"

namespace felicia {

class TCPServerSocket : public TCPSocket {
 public:
  using AcceptCallback = base::RepeatingCallback<void(Status)>;
  using AcceptOnceInterceptCallback =
      base::OnceCallback<void(StatusOr<std::unique_ptr<net::TCPSocket>>)>;

  TCPServerSocket();
  ~TCPServerSocket();

  const std::vector<std::unique_ptr<StreamSocket>>& accepted_sockets() const;

  StatusOr<ChannelDef> Listen();

  void AcceptLoop(AcceptCallback callback);
  void AcceptOnceIntercept(AcceptOnceInterceptCallback callback);

  void AddSocket(std::unique_ptr<net::TCPSocket> socket);
  void AddSocket(std::unique_ptr<TCPClientSocket> socket);
#if !defined(FEL_NO_SSL)
  void AddSocket(std::unique_ptr<SSLServerSocket> socket);
#endif  // !defined(FEL_NO_SSL)

  // Socket methods
  bool IsServer() const override;
  bool IsConnected() const override;

  // ChannelImpl methods
  // Write the |buffer| to the |accepted_sockets_|. If it succeeds to write
  // all the sockets, then callback with Status::OK(), otherwise callback
  // with the |write_result_|, which is recorded at every time finishing
  // write.
  void WriteAsync(scoped_refptr<net::IOBuffer> buffer, int size,
                  StatusOnceCallback callback) override;
  void ReadAsync(scoped_refptr<net::GrowableIOBuffer> buffer, int size,
                 StatusOnceCallback callback) override;

 private:
  int DoAccept();
  void DoAcceptLoop();
  void HandleAccpetResult(int result);
  void OnAccept(int result);

  void OnWrite(Status s);

  AcceptCallback accept_callback_;
  AcceptOnceInterceptCallback accept_once_intercept_callback_;

  net::IPEndPoint accepted_endpoint_;
  std::unique_ptr<net::TCPSocket> accepted_socket_;
  std::vector<std::unique_ptr<StreamSocket>> accepted_sockets_;

  StreamSocketBroadcaster broadcaster_;

  DISALLOW_COPY_AND_ASSIGN(TCPServerSocket);
};

}  // namespace felicia

#endif  // FELICIA_CORE_CHANNEL_SOCKET_TCP_SERVER_SOCKET_H_