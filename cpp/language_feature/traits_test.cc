/**************************************************************************************************
 * @File:        traits_test.cc
 * @Author:      moshouhua
 * @Date:        2022/12/06 22:44:45
 * @Description:
 **************************************************************************************************/

#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <typeinfo>

template <typename T, typename I>
void function(T a, I b) {
  if (typeid(a) == typeid(b)) {
    std::cout << "a.type == b.type, type: " << typeid(a).name() << std::endl;
  } else {
    std::cout << "a.type: " << typeid(a).name() << " b.type: " << typeid(b).name() << std::endl;
  }
}

class Foo {
 public:
  Foo() {}
  Foo(std::string name) : name_(name) {}
  std::string Name() { return name_; }
  void Print() {
    auto func = []() { std::cout << "this is lambda function" << std::endl; };
    func();
  }

 private:
  std::string name_;
};

void placement_new(void* addr, std::string name) { new (addr) Foo(name); }

template <typename T>
void Bar(T a) {
  std::cout << a << std::endl;
}
template <typename T, typename... Args>
void Bar(T a, Args... args) {
  std::cout << a << ", ";
  Bar(std::forward<Args>(args)...);
}

void init_test(std::initializer_list<int> args) {
  for (auto iter = args.begin(); iter != args.end(); ++iter) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
}

enum class 八卦 : int64_t {
  坎,  // 北
  坤,  // 西南
  震,  // 东
  巽,  // 东南
  中,
  乾,  // 西北
  兑,  // 西
  艮,  // 东北
  离   // 南
};

void lambda() {
  std::function<int(int)> fib;
  fib = [&](int n) -> int {
    if (n == 0) return 0;
    if (n == 1 || n == 2) {
      return 1;
    }
    return fib(n - 1) + fib(n - 2);
  };
  std::cout << "fib(7): " << fib(7) << std::endl;
}

int main() {
  function(1, 1);
  function<std::string, int>("test", 2);

  void* addr = malloc(sizeof(Foo));
  placement_new(addr, "test");
  std::cout << reinterpret_cast<Foo*>(addr)->Name() << std::endl;
  free(addr);

  Foo* ptr = nullptr;
  ptr->Print();

  Bar(1, 2, "3", "four");

  init_test({9, 8, 7, 1, 2, 3});

  八卦 a = 八卦::乾;
  std::cout << "乾: " << int64_t(a) << std::endl;

  lambda();

  return 0;
}