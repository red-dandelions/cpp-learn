#include "network_utils.h"

#include <fcntl.h>
#include <glog/logging.h>

namespace reactor {

void SetNonblock(int32_t fd) {
  int32_t flags = fcntl(fd, F_GETFL, 0);
  CHECK(flags != -1) << "fcntl get error!";
  flags |= O_NONBLOCK;
  CHECK(fcntl(fd, F_SETFL, flags) != -1) << "fcntl set error!";
}

}  // namespace reactor