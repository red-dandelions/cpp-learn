#include "cpp/examples/blocking_queue.h"

#include "gtest/gtest.h"

#include <memory>
#include <string>

TEST(BlockingQueueTest, BlockingQueue) {
  struct Data {
    size_t data;
    std::string name;
  };

  std::unique_ptr<examples::BlockingQueue<Data*>> bq(new examples::BlockingQueue<Data*>(10));
  ASSERT_TRUE(bq->Empty());
  for (size_t i = 0; i < 10; ++i) {
    Data d = {.data = i, .name = "name: " + std::to_string(i)};
    bq->Enqueue(&d);
  }
  ASSERT_TRUE(bq->Full(10));
  ASSERT_FALSE(bq->Empty());
  ASSERT_EQ(bq->Size(), 10);
  Data d = {.data = 11, .name = "name: 11"};
  ASSERT_FALSE(bq->TryEnqueue(&d, 10));
  ASSERT_EQ(bq->Size(), 10);
  bq->ForceEnqueue(&d);
  ASSERT_EQ(bq->Size(), 11);
  bq->Dequeue();
  bq->Dequeue();
  ASSERT_FALSE(bq->Full(10));
  Data head = {.data = 12, .name = "name: 12"};
  bq->EnqueueFront(&head);
  Data* res = bq->Dequeue();
  ASSERT_EQ(res->data, 12);
  ASSERT_EQ(res->name, "name: 12");
  for (size_t i = 0; i < 9; ++i) {
    bq->TryDequeue(&res, 10);
  }
  ASSERT_TRUE(bq->Empty());
  ASSERT_FALSE(bq->TryDequeue(&res, 10));
}