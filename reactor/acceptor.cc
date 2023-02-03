#include "acceptor.h"

#include <arpa/inet.h>
#include <glog/logging.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

namespace reactor {

AcceptorResult Acceptor::Accept(int32_t fd) {
  sockaddr_in client;
  socklen_t client_len = sizeof(client);
  int32_t client_fd = accept(fd, reinterpret_cast<sockaddr *>(&client), &client_len);

  if (client_fd == -1) {
    LOG(ERROR) << "accept failed, error: " << std::strerror(errno);
  }

  char ip[16];
  CHECK(inet_ntop(AF_INET, (const void *)&client.sin_addr, ip, 16) != nullptr)
      << "inet_ntop failed, error: " << std::strerror(errno);

  std::string port = std::to_string(ntohs(client.sin_port));

  AcceptorResult ret;
  ret.fd = client_fd;
  ret.address = std::string(ip) + ":" + port;

  return ret;
}

}  // namespace reactor