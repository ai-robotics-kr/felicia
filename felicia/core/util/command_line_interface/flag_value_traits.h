// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FELICIA_CORE_UTIL_COMMAND_LINE_INTERFACE_FLAG_VALUE_TRAITS_H_
#define FELICIA_CORE_UTIL_COMMAND_LINE_INTERFACE_FLAG_VALUE_TRAITS_H_

namespace felicia {

// For the flag which has a default value.
template <typename T>
class DefaultValueTraits {
 public:
  DefaultValueTraits(T* dst, T default_value) : dst_(dst) {
    DCHECK(dst);
    set_value(default_value);
  }
  DefaultValueTraits(const DefaultValueTraits& other) = default;
  DefaultValueTraits& operator=(const DefaultValueTraits& other) = default;

  void set_value(T value) { *dst_ = value; }
  T value() const { return *dst_; }
  bool is_set() const { return true; }

  // This is for python binding. Do not call from c++ side.
  void release() { delete dst_; }

 private:
  T* dst_;
};

// If the flag doesn't set, it returns initial value T{}.
template <typename T>
class InitValueTraits {
 public:
  explicit InitValueTraits(T* dst) : dst_(dst), is_set_(false) {
    DCHECK(dst);
    *dst_ = T{};
  }
  InitValueTraits(const InitValueTraits& other) = default;
  InitValueTraits& operator=(const InitValueTraits& other) = default;

  void set_value(T value) {
    *dst_ = value;
    is_set_ = true;
  }
  T value() const { return *dst_; }
  bool is_set() const { return is_set_; }

  // This is for python binding. Do not call from c++ side.
  void release() { delete dst_; }

 private:
  T* dst_;
  bool is_set_;
};

}  // namespace felicia

#endif  // FELICIA_CORE_UTIL_COMMAND_LINE_INTERFACE_FLAG_VALUE_TRAITS_H_
