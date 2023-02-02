#include "acceptor.h"
#include <glog/logging.h>
#include <string>
#include <sys/socket.h>

namespace reactor {

int32_t Acceptor::Accept(int32_t fd) {
  int32_t ret = accept(fd, nullptr, nullptr);

  if (ret == -1) {
    LOG(ERROR) << "accept failed, error: " << std::strerror(errno);
  }

  return ret;
}

}  // namespace reactor