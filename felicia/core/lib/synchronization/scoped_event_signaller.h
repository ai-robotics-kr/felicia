// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_CORE_LIB_SYNCHRONIZATION_SCOPED_EVENT_SIGNALLER_H_
#define FELICIA_CORE_LIB_SYNCHRONIZATION_SCOPED_EVENT_SIGNALLER_H_

#include "third_party/chromium/base/synchronization/waitable_event.h"

namespace felicia {

struct ScopedEventSignaller {
  ScopedEventSignaller(base::WaitableEvent* event) : event(event) {}
  ~ScopedEventSignaller() {
    if (event) event->Signal();
  }

  base::WaitableEvent* event;
};

}  // namespace felicia

#endif  // FELICIA_CORE_LIB_SYNCHRONIZATION_SCOPED_EVENT_SIGNALLER_H_