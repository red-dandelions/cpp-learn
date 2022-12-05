#include "cpp/class.h"

int main() {
  using namespace cpp;

  Foo a;
  std::cout << " a name: " << a.Name() << std::endl;

  std::string str = "Bar";
  Foo b(str);
  std::cout << " b name: " << b.Name() << std::endl;

  Foo c=a;
  std::cout << " c name: " << c.Name() << std::endl;

  c = b;
  std::cout << " c name: " << c.Name() << std::endl;

  Foo d(std::move(c));
  std::cout << " d name: " << d.Name() << std::endl;

  d = std::move(a);
  std::cout << " d name: " << d.Name() << std::endl;
  return 0;

}