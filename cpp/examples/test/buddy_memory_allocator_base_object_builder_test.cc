#include "cpp/examples/buddy_memory_allocator_base_object_builder.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

using namespace examples;

namespace {
struct TestUtil {
  std::unique_ptr<uint8_t[]> pool;
  std::vector<uint8_t> buffer;
  BuddyMemoryAllocatorBaseObjectBuilder* allocator() {
    return reinterpret_cast<BuddyMemoryAllocatorBaseObjectBuilder*>(buffer.data());
  }
  explicit TestUtil(size_t pool_size, uint32_t num_order) {
    pool = std::make_unique<uint8_t[]>(pool_size);
    ObjectBuilder builder;
    auto offset =
        BuddyMemoryAllocatorBaseObjectBuilder::create(builder, pool.get(), pool_size, num_order);
    CHECK_EQ(offset, 0);
    buffer = builder.buffer();
  }
};
}  // namespace

TEST(BuddyMemoryAllocatorBaseObjectBuilder, create) {
  ObjectBuilder builder;
  const uint32_t num_order = 4;
  const size_t pool_size = 2 << num_order;
  const auto pool = std::make_unique<uint8_t[]>(pool_size);
  int32_t object =
      BuddyMemoryAllocatorBaseObjectBuilder::create(builder, pool.get(), pool_size, num_order);

  auto allocator = builder.as<BuddyMemoryAllocatorBaseObjectBuilder>(object);
  ASSERT_EQ(num_order, allocator->num_order());
  ASSERT_EQ(pool_size, allocator->pool_size());
}

TEST(BuddyMemoryAllocatorBaseObjectBuilder, allocate) {
  {
    TestUtil mem_pool(2, 1);
    auto addr1 = mem_pool.allocator()->allocate(1);
    auto addr2 = mem_pool.allocator()->allocate(1);

    ASSERT_EQ(addr1, mem_pool.pool.get());
    ASSERT_EQ(addr2, nullptr);
  }

  {
    TestUtil mem_pool(2, 2);

    auto addr1 = mem_pool.allocator()->allocate(1);
    auto addr2 = mem_pool.allocator()->allocate(1);

    ASSERT_EQ(addr1, mem_pool.pool.get());
    ASSERT_EQ(addr2, mem_pool.pool.get() + 1);

    auto addr3 = mem_pool.allocator()->allocate(1);
    ASSERT_EQ(addr3, nullptr);
  }

  {
    TestUtil mem_pool(2, 2);

    auto addr1 = mem_pool.allocator()->allocate(1);
    auto addr2 = mem_pool.allocator()->allocate(1);

    ASSERT_EQ(addr1, mem_pool.pool.get());
    ASSERT_EQ(addr2, mem_pool.pool.get() + 1);

    mem_pool.allocator()->deallocate(addr1, 1);
    auto addr3 = mem_pool.allocator()->allocate(1);
    ASSERT_EQ(addr3, addr1);
  }
}