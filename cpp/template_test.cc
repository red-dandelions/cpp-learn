#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

template <typename T>
class Foo {
 public:
  Foo() {}
  Foo(T t) : value_(t) {}

  T Value() { return value_; }

 private:
  T value_;
};

// 特例化 float 类型
template <>
class Foo<float> {
 public:
  Foo() {}
  Foo(float t) : value_(t) {}
  float Value() { return value_ + 1.0; }

 private:
  float value_;
};

template <typename T, typename I>
class Bar {
 public:
  Bar(T t_val, I i_val) : t_value_(t_val), i_value_(i_val) {}
  T TValue() { return t_value_; }
  I IValue() { return i_value_; }

 private:
  T t_value_;
  I i_value_;
};

// 偏特化
template <typename T>
class Bar<T, float> {
 public:
  Bar(T t_val, float i_val) : t_value_(t_val), i_value_(i_val) {}
  T TValue() { return t_value_; }
  float IValue() { return i_value_ + 1; }

 private:
  T t_value_;
  float i_value_;
};

// 函数不能偏特化，只能全特化
template <typename T, typename I>
void function(T a, I b) {
  std::cout << "T I\n";
}

template <>
void function(int a, float b) {
  std::cout << "int float\n";
}

int main() {
  // 需要注意的是不指定类型，由编译器推断的话，不会匹配到特化版本
  Foo<std::string> foo("test");
  Foo<float> foo1(0.0);
  std::cout << foo.Value() << std::endl;   // test
  std::cout << foo1.Value() << std::endl;  // 1

  Bar<std::string, std::string> bar("test", "Test");
  Bar<std::string, float> bar1("test", 1.5);
  std::cout << "t: " << bar.TValue() << " i: " << bar.IValue() << std::endl;    // t: test i: Test
  std::cout << "t: " << bar1.TValue() << " i: " << bar1.IValue() << std::endl;  // t: test i: 2.5

  function<std::string, std::string>("a", "b");  // T I
  function<int, float>(4, 4.5);                  // int float;
  function(4, 6.5);                              // T I

  return 0;
}