#include "glog/logging.h"

#include <fcntl.h>

void set_nonblock(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  CHECK(flags != -1) << "fcntl get error!";
  flags |= O_NONBLOCK;
  CHECK(fcntl(fd, F_SETFL, flags) != -1) << "fcntl set error!";
}