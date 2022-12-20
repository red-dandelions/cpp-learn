#include "cpp/datastructure/queue/ringbuffer.h"

#include "gtest/gtest.h"

TEST(RingBufferTest, init) {
  datastructure::queue::RingBuffer ringbuffer;
  ASSERT_EQ(ringbuffer.init(), true);
}

TEST(RingBufferTest, push_pop) {
  datastructure::queue::RingBuffer ringbuffer;
  ASSERT_EQ(ringbuffer.init(40), true);

  datastructure::queue::RingBuffer::Node node1;
  node1.size = 5;
  node1.data = std::vector<uint8_t>(node1.size, 1);
  datastructure::queue::RingBuffer::Node node2;
  node2.size = 5;
  node2.data = std::vector<uint8_t>(node2.size, 2);
  datastructure::queue::RingBuffer::Node node3;
  node3.size = 5;
  node3.data = std::vector<uint8_t>(node1.size, 3);

  ringbuffer.push(node1);
  ringbuffer.push(node2);
  ringbuffer.push(node3);

  auto res = ringbuffer.pop();

  ASSERT_EQ(node2.size, res.size);

  for (size_t i = 0; i < node2.size; ++i) {
    ASSERT_EQ(node2.data[i], res.data[i]);
  }

  res = ringbuffer.pop();
  ASSERT_EQ(node3.size, res.size);

  for (size_t i = 0; i < node3.size; ++i) {
    ASSERT_EQ(node3.data[i], res.data[i]);
  }
}