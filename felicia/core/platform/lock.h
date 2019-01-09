// Copyright (c) 2011 The Chromium Authors. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// =====================================================================
// Modifications copyright (C) 2019 felicia

#ifndef FELICIA_CORE_PLATFORM_LOCK_H_
#define FELICIA_CORE_PLATFORM_LOCK_H_

#include "felicia/core/lib/base/export.h"
#include "felicia/core/lib/base/logging.h"
#include "felicia/core/lib/base/macros.h"
#include "felicia/core/lib/base/platform.h"
#include "felicia/core/lib/base/thread_annotations.h"
#include "felicia/core/platform/lock_impl.h"
#include "felicia/core/platform/platform_thread.h"

namespace felicia {

// A convenient wrapper for an OS specific critical section.  The only real
// intelligence in this class is in debug mode for the support for the
// AssertAcquired() method.
class LOCKABLE EXPORT Lock {
 public:
#if !DCHECK_IS_ON()
  // Optimized wrapper implementation
  Lock() : lock_() {}
  ~Lock() {}

  // TODO(lukasza): https://crbug.com/831825: Add EXCLUSIVE_LOCK_FUNCTION
  // annotation to Acquire method and similar annotations to Release, Try and
  // AssertAcquired methods (here and in the #else branch).
  void Acquire() { lock_.Lock(); }
  void Release() { lock_.Unlock(); }

  // If the lock is not held, take it and return true. If the lock is already
  // held by another thread, immediately return false. This must not be called
  // by a thread already holding the lock (what happens is undefined and an
  // assertion may fail).
  bool Try() { return lock_.Try(); }

  // Null implementation if not debug.
  void AssertAcquired() const {}
#else
  Lock();
  ~Lock();

  // NOTE: We do not permit recursive locks and will commonly fire a DCHECK() if
  // a thread attempts to acquire the lock a second time (while already holding
  // it).
  void Acquire() {
    lock_.Lock();
    CheckUnheldAndMark();
  }
  void Release() {
    CheckHeldAndUnmark();
    lock_.Unlock();
  }

  bool Try() {
    bool rv = lock_.Try();
    if (rv) {
      CheckUnheldAndMark();
    }
    return rv;
  }

  void AssertAcquired() const;
#endif  // DCHECK_IS_ON()

  // Both Windows and POSIX implementations of ConditionVariable need to be
  // able to see our lock and tweak our debugging counters, as they release and
  // acquire locks inside of their condition variable APIs.
  friend class ConditionVariable;

 private:
#if DCHECK_IS_ON()
  // Members and routines taking care of locks assertions.
  // Note that this checks for recursive locks and allows them
  // if the variable is set.  This is allowed by the underlying implementation
  // on windows but not on Posix, so we're doing unneeded checks on Posix.
  // It's worth it to share the code.
  void CheckHeldAndUnmark();
  void CheckUnheldAndMark();

  // All private data is implicitly protected by lock_.
  // Be VERY careful to only access members under that lock.
  felicia::PlatformThreadRef owning_thread_ref_;
#endif  // DCHECK_IS_ON()

  // Platform specific underlying lock implementation.
  internal::LockImpl lock_;

  DISALLOW_COPY_AND_ASSIGN(Lock);
};

// A helper class that acquires the given Lock while the AutoLock is in scope.
class SCOPED_LOCKABLE AutoLock {
 public:
  struct AlreadyAcquired {};

  explicit AutoLock(Lock& lock) EXCLUSIVE_LOCK_FUNCTION(lock) : lock_(lock) {
    lock_.Acquire();
  }

  AutoLock(Lock& lock, const AlreadyAcquired&) EXCLUSIVE_LOCKS_REQUIRED(lock)
      : lock_(lock) {
    lock_.AssertAcquired();
  }

  ~AutoLock() UNLOCK_FUNCTION() {
    lock_.AssertAcquired();
    lock_.Release();
  }

 private:
  Lock& lock_;
  DISALLOW_COPY_AND_ASSIGN(AutoLock);
};

// AutoUnlock is a helper that will Release() the |lock| argument in the
// constructor, and re-Acquire() it in the destructor.
class AutoUnlock {
 public:
  explicit AutoUnlock(Lock& lock) : lock_(lock) {
    // We require our caller to have the lock.
    lock_.AssertAcquired();
    lock_.Release();
  }

  ~AutoUnlock() { lock_.Acquire(); }

 private:
  Lock& lock_;
  DISALLOW_COPY_AND_ASSIGN(AutoUnlock);
};

}  // namespace felicia

#endif  // FELICIA_CORE_PLATFORM_LOCK_H_
