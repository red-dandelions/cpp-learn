#pragma once

#include "spin_lock.h"

#include <vector>

namespace examples {

class BuddyMemoryAllocator {
 public:
  explicit BuddyMemoryAllocator(uint8_t* pool_addr, size_t pool_size, uint32_t num_order)
      : pool_addr_(pool_addr), pool_size_(pool_size), num_order_(num_order) {
    init_pool();
  }

  // 分配内存
  void* allocate(size_t size);
  // 释放内存
  void deallocate(const void* addr, size_t size);

  uint32_t num_order() const { return num_order_; }
  size_t pool_size() const { return pool_size_; }

 private:
  // 初始化 pool_
  void init_pool();
  // 更新 pool_
  void update_pool(size_t block_idx);

  SpinLock mutex_;

  uint8_t* const pool_addr_;
  const size_t pool_size_;
  const uint32_t num_order_;

  // 可分配的最小的块大小
  size_t min_block_size_ = 0;

  // 满二叉树
  std::vector<size_t> pool_;
};

}  // namespace examples