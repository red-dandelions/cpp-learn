#ifndef __CPP_CLASS_H__
#define __CPP_CLASS_H__

#include <string>
#include <iostream>

namespace cpp {
// 构造函数、
// 复制构造函数、赋值运算符
// 移动构造函数、移动运算符

class Foo {
 public:
  // 无参构造函数
  Foo() {
    name_ = "default";
  }
  // 有参构造函数
  Foo(std::string name) : name_(name) {
  }
 
  // 复制构造函数
  Foo(const Foo& foo) : name_(foo.name_) {
  }
  // 赋值运算符
  Foo& operator=(const Foo& rhs) {
    name_ = rhs.name_;
    return *this;
  }

  // 移动构造函数 && 表示传进来的是右值
  Foo(Foo&& foo) : name_(foo.name_) {
    foo.name_ = "";
  }
  Foo& operator=(Foo&& rhs) {
    name_ = rhs.name_;
    return *this;
  }

  std::string Name() {
    return name_;
  }

 private:
  std::string name_;
};



} // namespace cpp

#endif