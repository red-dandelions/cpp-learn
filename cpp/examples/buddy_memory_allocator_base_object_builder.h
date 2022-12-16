#pragma once

#include "object_builder.h"
#include "spin_lock.h"

namespace examples {

class BuddyMemoryAllocatorBaseObjectBuilder {
 public:
  static int32_t create(ObjectBuilder& builder, void* pool_addr, size_t pool_size,
                        uint32_t num_order);

  void* allocate(size_t size);
  void deallocate(const void* addr, size_t size);

  uint32_t num_order() const { return num_order_; }
  size_t pool_size() const { return pool_size_; }

 private:
  explicit BuddyMemoryAllocatorBaseObjectBuilder(uint8_t* pool_addr, size_t pool_size,
                                                 uint32_t num_order)
      : pool_addr_(pool_addr), pool_size_(pool_size), num_order_(num_order) {}

  // init pool_
  void init_pool();

  // update pool_
  void update_pool(size_t block_idx);

  SpinLock mutex_;
  uint8_t* pool_addr_;
  const size_t pool_size_;
  const uint32_t num_order_;
  size_t min_block_size_ = 0;
  Pointer<size_t> pool_;
};

}  // namespace examples