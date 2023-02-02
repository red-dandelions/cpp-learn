#pragma once

#include <cinttypes>

namespace reactor {

class Acceptor {
 public:
  Acceptor() {}
  ~Acceptor() {}

  int32_t Accept(int32_t fd);
};

}  // namespace reactor