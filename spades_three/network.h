#pragma once

#include "glog/logging.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <string>
#include <sys/socket.h>

namespace spades_three {

void SetNonblock(int32_t fd) {
  int32_t flags = fcntl(fd, F_GETFL, 0);
  CHECK(flags != -1) << "fcntl get error!";
  flags |= O_NONBLOCK;
  CHECK(fcntl(fd, F_SETFL, flags) != -1) << "fcntl set error!";
}

int32_t Connect(const std::string& ip, const std::string port) {
  int32_t socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  CHECK(socket_fd != -1) << "socket failed, error: " << std::strerror(errno);

  // 设置非阻塞
  SetNonblock(socket_fd);

  sockaddr_in server;
  std::memset(&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_port = htons(std::stoi(port));
  CHECK(inet_pton(AF_INET, ip.c_str(), &server.sin_addr) == 1)
      << "inet_pton failed. error: " << std::strerror(errno);

  CHECK(connect(socket_fd, (sockaddr*)(&server), sizeof(server)) != -1)
      << "connect failed, error: " << std::strerror(errno);

  return socket_fd;
}

}  // namespace spades_three