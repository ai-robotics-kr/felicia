#ifndef FELICIA_CORE_LIB_CONTAINERS_STRING_VECTOR_H_
#define FELICIA_CORE_LIB_CONTAINERS_STRING_VECTOR_H_

#include <string>
#include <type_traits>

#include "third_party/chromium/base/containers/checked_iterators.h"
#include "third_party/chromium/base/logging.h"

#include "felicia/core/lib/base/export.h"

namespace felicia {

// StringVector is a container which has std::string inside, but is able to
// be used like std::vector. It is used c++ implementation against
// bytes type of protobuf.
class EXPORT StringVector {
 public:
  template <typename T>
  class Iterator {
   public:
    using iterator = base::CheckedRandomAccessIterator<T>;
    using const_iterator = base::CheckedRandomAccessConstIterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    explicit Iterator(StringVector& vector) noexcept : vector_(vector) {}
    void operator=(const Iterator& iterator) { vector_ = iterator.vector_; }

    iterator begin() const noexcept {
      return iterator(vector_.cast<T*>(),
                      vector_.cast<T*>() + vector_.size<T>());
    }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() const noexcept {
      return iterator(vector_.cast<T*>(),
                      vector_.cast<T*>() + vector_.size<T>(),
                      vector_.cast<T*>() + vector_.size<T>());
    }
    const_iterator cend() const noexcept { return end(); }

    reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return rend(); }

   private:
    StringVector& vector_;
  };

  StringVector() = default;
  StringVector(const void* s, size_t n)
      : data_(reinterpret_cast<const char*>(s), n) {}
  StringVector(const std::string& data) : data_(data) {}
  StringVector(std::string&& data) noexcept : data_(std::move(data)) {}
  StringVector(const StringVector& other) = default;
  StringVector(StringVector&& other) noexcept : data_(std::move(other.data_)) {}
  ~StringVector() = default;

  StringVector& operator=(const StringVector& other) = default;
  StringVector& operator=(StringVector&& other) = default;

  template <typename T>
  void push_back(const T& value) {
    size_t size = data_.size();
    data_.resize(size + sizeof(T));
    char* ptr = const_cast<char*>(data_.data() + size);
    new (ptr) T(value);
  }

  template <typename T>
  void push_back(T&& value) {
    size_t size = data_.size();
    data_.resize(size + sizeof(T));
    char* ptr = const_cast<char*>(data_.data() + size);
    new (ptr) std::decay_t<T>(std::forward<T>(value));
  }

  template <typename R>
  std::enable_if_t<std::is_pointer<R>::value, R> cast() const noexcept {
    return reinterpret_cast<R>(data_.data());
  }

  template <typename R>
  std::enable_if_t<std::is_pointer<R>::value, R> cast() noexcept {
    return reinterpret_cast<R>(const_cast<char*>(data_.data()));
  }

  template <typename R>
  R& at(size_t idx) {
    CHECK(size<R>() > idx);
    R* ptr = cast<R*>() + idx;
    return *ptr;
  }

  template <typename R>
  const R& at(size_t idx) const {
    CHECK(size<R>() > idx);
    const R* ptr = cast<const R*>() + idx;
    return *ptr;
  }

  size_t raw_size() const noexcept { return data_.size(); }

  template <typename T>
  size_t size() const noexcept {
    return data_.size() / sizeof(T);
  }

  bool empty() const noexcept { return data_.size() == 0; }

  void reserve(size_t n) { data_.reserve(n); }

  void resize(size_t n) { data_.resize(n); }

  void clear() { data_.clear(); }

  const std::string& data() const& noexcept { return data_; }
  std::string&& data() && noexcept { return std::move(data_); }

  template <typename R>
  Iterator<R> Iterated() noexcept {
    return Iterator<R>(*this);
  }

 private:
  std::string data_;
};

}  // namespace felicia

#endif  // FELICIA_CORE_LIB_CONTAINERS_STRING_VECTOR_H_