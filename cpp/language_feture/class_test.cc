#include <iostream>
#include <string>

// 构造函数、
// 复制构造函数、赋值运算符
// 移动构造函数、移动运算符

class Foo {
 public:
  // 无参构造函数
  Foo() {
    name_ = "default";
    std::cout << "use default constructor\n";
  }
  // 有参构造函数
  Foo(std::string name) : name_(name) { std::cout << "use param constructor\n"; }

  // 复制构造函数
  Foo(const Foo& foo) : name_(foo.name_) { std::cout << "use copy constructor\n"; }
  // 赋值运算符
  Foo& operator=(const Foo& rhs) {
    std::cout << "use assignment operator =\n";
    name_ = rhs.name_;
    return *this;
  }

  // 移动构造函数 && 表示传进来的是右值
  Foo(Foo&& foo) : name_(foo.name_) {
    std::cout << "use move constructor\n";
    foo.name_ = "";
  }
  Foo& operator=(Foo&& rhs) {
    std::cout << "use move operator =\n";
    name_ = rhs.name_;
    return *this;
  }

  std::string Name() { return name_; }

 private:
  std::string name_;
};

int main() {
  Foo a;                // use default constructor
  Foo b("Bar");         // use param constructor
  Foo c = a;            // use copy constructor
  c = b;                // use assignment operator =
  Foo d(std::move(a));  // use move constructor
  d = std::move(b);     // use move operator =

  return 0;
}