#include "examples/protobuf/file/hello.pb.h"

#include <iostream>
#include <string>

int main() {
  vscode::examples::protobuf::Hello h;
  h.set_name("hello");

  std::string str = h.name();

  std::cout << str << std::endl;

  return 0;
}