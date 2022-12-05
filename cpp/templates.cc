#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
// TODO
// Args... args
// std::forward<Args&...>(args...)

template <typename T>
class Foo {
 public:
  Foo(){}
  Foo(T t) : value_(t) {}

  T Value() { return value_; }
 private:
  T value_;
};

// 特例化 float 类型 or 全特化
template <>
class Foo<float> {
 public:
  Foo(){}
  Foo(float t) : value_(t) {}

  float Value() { 
    float a = 1.0;
    std::cout << "this is float" << std::endl;
    return value_ + a;
  }

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


// 偏特化 or 部分特化
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

// 函数不能偏特化（部分特化），只能全特化（实例化)
template <typename T, typename I>
void function(T a, I b) {
  std::cout << "T I\n";
} 

template <>
void function(int a, float b) {
  std::cout << "int float\n"; 
}



int main() {
  Foo<std::string> foo("test");
  std::cout << foo.Value() << std::endl;

  Foo<float> foo1(0.0);
  std::cout << foo1.Value() << std::endl;

  Bar<std::string, std::string> bar("test", "Test");
  std::cout << "t: " << bar.TValue() << " i: " << bar.IValue() << std::endl;

  Bar<std::string, float> bar1("test", 1.5);
  std::cout << "t: " << bar1.TValue() << " i: " << bar1.IValue() << std::endl;

  function<std::string, std::string>("a", "b"); // 不指定，有编译器推断类型，不能到特化版本
  function<int, float>(4, 4.5);
  function(4, 6.5);

  return 0;
}