#pragma once

#include "pointer.h"

#include <vector>

namespace examples {

class ObjectBuilder {
 public:
  // 获取存放数据的 buffer
  const std::vector<uint8_t>& buffer() const { return buffer_; }

  // 获取在 buffer 中 offset 偏移量存放的 T 类型的指针
  template <typename T>
  T* as(int32_t offset) {
    return reinterpret_cast<T*>(buffer_.data() + offset);
  }

  // 为类型 T 分配空间
  template <typename T>
  int32_t allocate() {
    // 将 buffer 中的空间对齐 alignof(T)，然后返回 T 在 buffer 中的偏移量
    const int32_t offset = compute_aligned_size(buffer_.size(), alignof(T));
    buffer_.resize(offset + sizeof(T));
    return offset;
  }
  template <typename T>
  int32_t allocate(uint32_t num_elements) {
    const int32_t offset = compute_aligned_size(buffer_.size(), alignof(T));
    buffer_.resize(offset + sizeof(T) * num_elements);
    return offset;
  }

  // @brief 更新对象的指针成员
  //
  // @param object 对象的位置
  // @param data_ptr 待更新的数据成员的指针
  // @param offset 要写入的 offset。offset 是在 builder 中的 offset，而不是相较于某个对象的 offset
  // 调用这个接口的指针成员必须为 Pointer<T> data_ptr
  template <typename ObjectT, typename FieldT>
  void update_object_pointer_member(int32_t object, Pointer<FieldT> ObjectT::*data_ptr,
                                    int32_t offset) {
    const ObjectT* dummy_ptr = nullptr;
    const intptr_t offset_inner_object = reinterpret_cast<intptr_t>(&(dummy_ptr->*data_ptr));

    // 指针在 buffer 中的位置
    const int32_t pointer_address = object + offset_inner_object;

    // 写入相对位置，这里 Pointer 有个成员 offset_ = offset - pointer_address，指向真正的地址
    *reinterpret_cast<int32_t*>(buffer_.data() + pointer_address) = offset - pointer_address;
  }

  // @brief 更新对象的某个普通成员
  //
  // @param object 对象的位置
  // @param data_ptr 待更新成员的指针
  // @param val 写入的数据
  template <typename ObjectT, typename FieldT>
  void update_object_normal_member(int32_t object, FieldT ObjectT::*data_ptr, FieldT val) {
    const ObjectT* dummy_ptr = nullptr;
    const intptr_t pos = reinterpret_cast<intptr_t>(&(dummy_ptr->*data_ptr));
    *reinterpret_cast<FieldT*>(buffer_.data() + object + pos) = val;
  }

 private:
  size_t compute_aligned_size(size_t size, size_t align_size) {
    size_t align_mask = align_size - 1;
    return (size + align_mask) & (~align_mask);
  }

  std::vector<uint8_t> buffer_;
};

}  // namespace examples