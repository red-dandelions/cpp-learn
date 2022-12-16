#include "cpp/examples/object_builder.h"

#include "gtest/gtest.h"

using namespace examples;

TEST(ObjectBuilder, update_object_normal_member) {
  struct TestObject {
    int32_t filed1 = 0;
    int32_t filed2 = 0;
  };

  ObjectBuilder builder;
  int32_t offset1 = builder.allocate<TestObject>();
  builder.update_object_normal_member(offset1, &TestObject::filed1, int32_t(1));
  builder.update_object_normal_member(offset1, &TestObject::filed2, int32_t(2));

  std::vector<uint8_t> buffer = builder.buffer();

  auto object = reinterpret_cast<const TestObject*>(buffer.data());
  ASSERT_EQ(object->filed1, 1);
  ASSERT_EQ(object->filed2, 2);
}

TEST(ObjectBuilder, update_object_pointer_member) {
  struct TestObject {
    int32_t value = 0;
    Pointer<TestObject> pointer;
  };

  ObjectBuilder builder;
  const int32_t object1 = builder.allocate<TestObject>();
  builder.update_object_normal_member(object1, &TestObject::value, int32_t(1));

  const int32_t object2 = builder.allocate<TestObject>();
  builder.update_object_normal_member(object2, &TestObject::value, int32_t(2));
  builder.update_object_pointer_member(object1, &TestObject::pointer, object2);

  std::vector<uint8_t> buffer = builder.buffer();
  auto object = reinterpret_cast<const TestObject*>(buffer.data());

  ASSERT_EQ(object->value, 1);
  ASSERT_EQ(object->pointer->value, 2);
}