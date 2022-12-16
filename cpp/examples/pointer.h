#pragma once

#include <cstdint>

namespace examples {

// 基于位移的指针
template <typename T>
class Pointer {
 public:
  // 获取真实地址
  T* address() {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(this) + offset_;
    return reinterpret_cast<T*>(ptr);
  }
  const T* address() const {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(this) + offset_;
    return reinterpret_cast<const T*>(ptr);
  }

  T* operator->() { return address(); }
  const T* operator->() const { return address(); }

  T& operator[](int32_t index) { return address()[index]; }
  const T& operator[](int32_t index) const { return address()[index]; }

 private:
  int32_t offset_;
};

}  // namespace examples