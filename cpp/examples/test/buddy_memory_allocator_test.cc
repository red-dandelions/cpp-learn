#include "cpp/examples/buddy_memory_allocator.h"

#include "gtest/gtest.h"

using namespace examples;

namespace {
struct TestUtil {
  std::unique_ptr<uint8_t[]> pool;
  std::unique_ptr<BuddyMemoryAllocator> allocator;

  explicit TestUtil(size_t pool_size, uint32_t num_order) {
    pool = std::make_unique<uint8_t[]>(pool_size);
    allocator.reset(new BuddyMemoryAllocator(pool.get(), pool_size, num_order));
  }
};
}  // namespace

TEST(BuddyMemoryAllocator, allocate) {
  {
    TestUtil mem_pool(2, 1);
    auto addr1 = mem_pool.allocator->allocate(1);
    auto addr2 = mem_pool.allocator->allocate(1);

    ASSERT_EQ(addr1, mem_pool.pool.get());
    ASSERT_EQ(addr2, nullptr);
  }

  {
    TestUtil mem_pool(2, 2);
    auto addr1 = mem_pool.allocator->allocate(1);
    auto addr2 = mem_pool.allocator->allocate(1);

    ASSERT_EQ(addr1, mem_pool.pool.get());
    ASSERT_EQ(addr2, mem_pool.pool.get() + 1);

    auto addr3 = mem_pool.allocator->allocate(1);
    ASSERT_EQ(addr3, nullptr);
  }

  {
    TestUtil mem_pool(2, 2);
    auto addr1 = mem_pool.allocator->allocate(1);
    auto addr2 = mem_pool.allocator->allocate(1);

    ASSERT_EQ(addr1, mem_pool.pool.get());
    ASSERT_EQ(addr2, mem_pool.pool.get() + 1);

    mem_pool.allocator->deallocate(addr1, 1);
    auto addr3 = mem_pool.allocator->allocate(1);
    ASSERT_EQ(addr3, addr1);
  }
}