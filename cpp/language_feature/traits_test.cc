/**************************************************************************************************
 * @File:        traits_test.cc
 * @Author:      moshouhua
 * @Date:        2022/12/06 22:44:45
 * @Description:
 **************************************************************************************************/

#include <cstdlib>
#include <functional>
#include <future>
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

#ifdef __APPLE__
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
#endif

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

void promese_future() {
  auto func1 = [](std::promise<int32_t>& p) {
    std::cout << "wait future\n";
    auto tp1 = std::chrono::steady_clock::now();
    auto f = p.get_future();
    auto v = f.get();
    auto tp2 = std::chrono::steady_clock::now();

    std::cout << "f: " << v << " wait "
              << std::chrono::duration_cast<std::chrono::seconds>(tp2 - tp1).count() << " s"
              << std::endl;
  };
  auto func2 = [](std::promise<int32_t>& p) {
    std::cout << "set promise\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    p.set_value(10);
  };
  std::promise<int32_t> p;
  std::thread t1(func1, std::ref(p));
  std::thread t2(func2, std::ref(p));
  t1.join();
  t2.join();
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

#ifdef __APPLE__
  八卦 a = 八卦::乾;
  std::cout << "乾: " << int64_t(a) << std::endl;
#endif

  lambda();

  promese_future();

  return 0;
}