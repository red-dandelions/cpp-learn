#include "cpp/examples/singleton.h"

#include "gtest/gtest.h"

class Example : public examples::Singleton<Example> {
 public:
  // 要有空参构造函数
  Example() {}
  Example(std::string name) : name_(name) {}
  ~Example() {}
  void SetName(std::string name) { name_ = name; }
  std::string GetName() { return name_; };

 private:
  std::string name_;
};

TEST(SingletonTest, Singleton) {
  // 创建单例
  Example::Instance("test");
  ASSERT_EQ(Example::Get()->GetName(), "test");

  Example::Get()->SetName("new_name");
  ASSERT_EQ(Example::Get()->GetName(), "new_name");

  // 已经创建过了，所以不会再次创建实例
  Example::Instance("test_name");
  ASSERT_EQ(Example::Get()->GetName(), "new_name");
}