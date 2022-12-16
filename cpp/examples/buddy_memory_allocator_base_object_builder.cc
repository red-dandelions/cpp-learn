#include "buddy_memory_allocator_base_object_builder.h"

#include "glog/logging.h"

#include <mutex>

namespace examples {
namespace {
// 计算满二叉树中第 block_idx 块的 block 大小
size_t compute_block_size(size_t pool_size, size_t block_idx) {
  size_t block_size = pool_size;
  size_t tmp = 0;
  size_t order_block_count = 1;
  while ((tmp = (tmp + order_block_count)) <= block_idx) {
    order_block_count *= 2;
    block_size /= 2;
  }
  return block_size;
}
// 向上取整到2的幂次
size_t rounded_up(size_t size) {
  size_t value = 1;
  while (value < size) {
    value <<= 1;
  }
  return value;
}
// 向上取整到 >= min_block_size
size_t rounded_up_to_min_block_size(size_t size, size_t min_block_size) {
  return std::max<size_t>(rounded_up(size), min_block_size);
}
// 计算在第几层
uint32_t compute_level(size_t pool_size, size_t block_size) {
  uint32_t level = 0;
  while (pool_size / 2 >= block_size) {
    level++;
    pool_size /= 2;
  }
  return level;
}
}  // namespace

int32_t BuddyMemoryAllocatorBaseObjectBuilder::create(ObjectBuilder& builder, void* pool_addr,
                                                      size_t pool_size, uint32_t num_order) {
  int32_t offset = builder.allocate<BuddyMemoryAllocatorBaseObjectBuilder>();

  // 使用 placement new 调用构造函数
  new (builder.as<BuddyMemoryAllocatorBaseObjectBuilder>(offset))
      BuddyMemoryAllocatorBaseObjectBuilder(reinterpret_cast<uint8_t*>(pool_addr), pool_size,
                                            num_order);

  int32_t pool_offset = builder.allocate<size_t>(1 << num_order);

  builder.update_object_pointer_member(offset, &BuddyMemoryAllocatorBaseObjectBuilder::pool_,
                                       pool_offset);

  builder.as<BuddyMemoryAllocatorBaseObjectBuilder>(offset)->init_pool();

  return offset;
}

void BuddyMemoryAllocatorBaseObjectBuilder::init_pool() {
  //|1024|512|512|256|256|256|256|...
  size_t* pool = pool_.address();        // pool 地址
  size_t order_block_size = pool_size_;  // 当前 order 对应的块大小

  for (uint32_t order = 0; order < num_order_; ++order) {
    uint32_t order_block_count = 1 << order;  // order 层的块数目
    // 为当前 order 层对应的块设置初值
    while (order_block_count--) {
      *pool++ = order_block_size;
    }
    order_block_size = order_block_size / 2;  // 下一层的块大小
  }

  min_block_size_ = order_block_size * 2;
}

void BuddyMemoryAllocatorBaseObjectBuilder::update_pool(size_t block_idx) {
  size_t* pool = pool_.address();
  size_t block_size = compute_block_size(pool_size_, block_idx);
  while (block_idx != 0) {
    const size_t parent_idx = (block_idx - 1) / 2;             // 父结点
    const size_t parent_block_size = block_size * 2;           // 父结点大小
    const size_t left_block_size = pool[parent_idx * 2 + 1];   // 左结点
    const size_t right_block_size = pool[parent_idx * 2 + 2];  // 右结点
    if (left_block_size + right_block_size == parent_block_size) {
      pool[parent_idx] = parent_block_size;  // 可以合并
    } else {
      // 不能合并，取两个子 block 的最大值
      pool[parent_idx] = std::max<size_t>(left_block_size, right_block_size);
    }
    // 向根结点递归
    block_idx = parent_idx;
    block_size = parent_block_size;
  }
}

void* BuddyMemoryAllocatorBaseObjectBuilder::allocate(size_t size) {
  // 申请的空间大小
  const size_t allocate_block_size = rounded_up_to_min_block_size(size, min_block_size_);
  // 计算在满二叉树第几层
  const uint32_t level = compute_level(pool_size_, allocate_block_size);
  size_t* const pool = pool_.address();

  std::lock_guard<SpinLock> lock(mutex_);

  // 判断是否有足够的内存
  if (pool[0] < allocate_block_size) {
    return nullptr;
  }

  // 查找合适的块大小，返回一个 pair， pair.first 为 block_index, pair.second 为地址
  const auto search_suitable_block = [this, pool, allocate_block_size](
                                         uint32_t target_level,
                                         size_t size) -> std::pair<size_t, uint8_t*> {
    uint8_t* addr = pool_addr_;
    size_t block_size = pool_size_;
    size_t block_idx = 0;

    uint32_t cur_level = 0;
    // 先从左子树查找，不满足则在右子树查找
    while (cur_level++ < target_level) {
      const size_t child_block_size = block_size / 2;

      const size_t left_child_block_idx = block_idx * 2 + 1;
      if (pool[left_child_block_idx] >= size) {
        block_idx = left_child_block_idx;
      } else {
        const size_t right_child_block_idx = left_child_block_idx + 1;
        CHECK_GE(pool[right_child_block_idx], size)
            << ", pool_size: " << pool_size_ << ", target_level: " << target_level
            << ", cur_level: " << cur_level << ", size: " << size
            << ", rounded_up_size: " << allocate_block_size;
        block_idx = right_child_block_idx;
        addr += child_block_size;
      }

      // 设置下一次迭代数据
      block_size = child_block_size;
    }

    return std::make_pair(block_idx, addr);
  };

  std::pair<uint32_t, uint8_t*> block_addr_pair = search_suitable_block(level, allocate_block_size);
  CHECK(block_addr_pair.second != nullptr);

  // 标记以分配
  pool[block_addr_pair.first] = 0;
  // 更新二叉树
  update_pool(block_addr_pair.first);
  // 返回分配的地址
  return block_addr_pair.second;
}

void BuddyMemoryAllocatorBaseObjectBuilder::deallocate(const void* addr, size_t size) {
  // 计算已申请的内存大小
  const size_t allocate_block_size = rounded_up_to_min_block_size(size, min_block_size_);
  const uint32_t level = compute_level(pool_size_, allocate_block_size);
  const size_t offset = static_cast<size_t>(reinterpret_cast<const uint8_t*>(addr) - pool_addr_);

  std::lock_guard<SpinLock> lock(mutex_);

  size_t block_idx = 0;
  size_t block_size = pool_size_;
  size_t cur_offset = 0;

  uint32_t cur_level = 0;
  while (cur_level++ < level) {
    const size_t child_block_size = block_size / 2;
    if (offset < cur_offset + child_block_size) {
      // 左子树中查找
      block_idx = block_idx * 2 + 1;
    } else {
      // 右子树中查找
      block_idx = block_idx * 2 + 2;
      cur_offset += child_block_size;
    }

    block_size = child_block_size;
  }

  // 更新满二叉树
  pool_[block_idx] = block_size;
  update_pool(block_idx);
}

}  // namespace examples