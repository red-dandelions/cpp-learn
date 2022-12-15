#include "glog/logging.h"

#include <arpa/inet.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

void client(std::string_view ip, int32_t port) {
  int32_t fd = socket(PF_INET, SOCK_STREAM, 0);
  CHECK(fd != -1) << "socket error: " << std::strerror(errno);

  sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_port = htons(port);
  inet_pton(AF_INET, ip.data(), &server.sin_addr);

  CHECK(connect(fd, reinterpret_cast<sockaddr*>(&server), sizeof(server)) != -1)
      << "connect error: " << std::strerror(errno);

  char buf[] = "hello, I am client!";

  CHECK(send(fd, buf, strlen(buf), 0) == strlen(buf))
      << "send: " << strlen(buf) << " bytes. error: " << std::strerror(errno);
}

int main() {
  std::string ip = "127.0.0.1";
  const int32_t port = 8888;

  client(ip, port);

  return 0;
}