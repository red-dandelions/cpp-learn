#include <dlfcn.h>
#include <functional>
#include <iostream>

class Foo {
 public:
  template <typename T, typename... Params>
  bool BindFunction(void* dl, const char* name, std::function<T(Params...)>* const func) {
    // 加载 c++ 的时候会有问题，主要是 c++ 支持重载，函数名不等于符号
    // 所以 c++ 需要加上 extern "C" {}
    // 或者 nm xxx.so | grep functin_name
    // 比如 int add_int(int a, int b);
    // nm add.so | grep add_init
    // xxxxxxx T __Z7add_intii (这里两道下换线，实际传入dlsym时，要去掉一个下划线) C
    // 语言中 函数名和符号是一样的 void* sym = dlsym(dl, name); // C
    void* sym = dlsym(dl, "_Z7add_intii");  // C++
    if (sym == nullptr) {
      std::cout << "sym == nullptr\n";
      return false;
    }
    *func = reinterpret_cast<T (*)(Params...)>(sym);
    return true;
  }

  void LoadSymbols(void* dl) {
    std::function<int(int, int)> add_int;
#define LoadSymbol(sym) BindFunction(dl, #sym, &sym)
    if (LoadSymbol(add_int)) {
      std::cout << "10 + 3 = " << add_int(10, 3) << std::endl;
    }
#undef LoadSymbol
  }

  void Load() {
    void* dl = dlopen("/Users/moshouhua/Desktop/cpp-learn/bazel-bin/templates/dynamic_lib/add.so",
                      RTLD_LAZY);
    if (dl == nullptr) {
      std::cout << "dl == nullptr\n";
      return;
    }
    LoadSymbols(dl);
  }
};

int main() {
  Foo foo;
  foo.Load();

  return 0;
}