#pragma once

#include <cinttypes>
#include <string>

namespace reactor {

struct AcceptorResult {
  int32_t fd;
  std::string address;
};

class Acceptor {
 public:
  Acceptor() {}
  ~Acceptor() {}

  AcceptorResult Accept(int32_t fd);
};

}  // namespace reactor