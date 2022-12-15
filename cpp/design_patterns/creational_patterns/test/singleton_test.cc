#include "cpp/design_patterns/creational_patterns/singleton.h"

#include "gtest/gtest.h"

TEST(Singleton, constructor) {
  using namespace design_patterns::creational_patterns;

  Singleton* intance1 = Singleton::Instance();
  Singleton* intance2 = Singleton::Instance();

  ASSERT_TRUE(intance1 != nullptr);
  ASSERT_EQ(intance1, intance2);
}