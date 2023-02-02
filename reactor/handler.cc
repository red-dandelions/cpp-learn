#include "handler.h"

#include <string>
#include <unistd.h>
#include <glog/logging.h>

namespace reactor {

int32_t Handler::Read() {
  int32_t bytes;
  std::string buffer;
  buffer.resize(1024);

  while ((bytes = read(fd_, (void *)buffer.c_str(), 1024)) != 0) {

  }

  LOG(INFO) << "read: " << buffer;
  return 0;
}

int32_t Handler::Write() {

}

} // namespace reactor