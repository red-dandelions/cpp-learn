#pragma once

#include <memory>
#include <string>

namespace reactor {

class Processor {
 public:
  Processor() {}
  ~Processor() {}

  std::string processor();
};

}  // namespace reactor