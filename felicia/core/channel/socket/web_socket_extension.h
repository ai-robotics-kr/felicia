// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_CORE_CHANNEL_SOCKET_WEB_SOCKET_EXTENSION_H_
#define FELICIA_CORE_CHANNEL_SOCKET_WEB_SOCKET_EXTENSION_H_

#include <memory>
#include <string>

#include "third_party/chromium/base/containers/flat_map.h"
#include "third_party/chromium/base/strings/string_tokenizer.h"

#include "felicia/core/channel/settings.h"

namespace felicia {

class PermessageDeflate;

class WebSocketExtensionInterface {
 public:
  WebSocketExtensionInterface();
  virtual ~WebSocketExtensionInterface();
  virtual bool Negotiate(base::StringTokenizer& params,
                         const channel::WSSettings& settings,
                         std::string* response) = 0;
  virtual void AppendResponse(std::string* response) const = 0;

  virtual bool IsPerMessageDeflate() const { return false; }
  PermessageDeflate* ToPermessageDeflate() {
    DCHECK(IsPerMessageDeflate());
    return reinterpret_cast<PermessageDeflate*>(this);
  }
};

class WebSocketExtension {
 public:
  WebSocketExtension();
  bool Negotiate(
      const std::string& extensions, const channel::WSSettings& settings,
      std::string* response,
      std::vector<WebSocketExtensionInterface*>* accepted_extensions);

 private:
  base::flat_map<std::string, std::unique_ptr<WebSocketExtensionInterface>>
      extensions_;
};

}  // namespace felicia

#endif  // FELICIA_CORE_CHANNEL_SOCKET_WEB_SOCKET_EXTENSION_H_