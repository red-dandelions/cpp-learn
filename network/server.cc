#include "glog/logging.h"

#include <arpa/inet.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

void server(std::string_view ip, int32_t port) {
  int fd = socket(PF_INET, SOCK_STREAM, 0);
  CHECK(fd != -1) << "socket error: " << std::strerror(errno);
  sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_port = htons(port);
  inet_pton(AF_INET, ip.data(), &server.sin_addr);

  CHECK(bind(fd, reinterpret_cast<sockaddr*>(&server), sizeof(server)) != -1)
      << "bind error: " << std::strerror(errno);

  CHECK(listen(fd, 5) != -1) << "listen error: " << std::strerror(errno);

  while (true) {
    sockaddr_in client;
    socklen_t cli_len;
    int client_fd = accept(fd, reinterpret_cast<sockaddr*>(&client), &cli_len);
    CHECK(client_fd != -1) << "accept error: " << std::strerror(errno);
    char buf[512];
    memset(buf, 0, sizeof(buf));
    while (read(client_fd, buf, 512) != 0) {
      LOG(INFO) << "data: " << buf;
    }
  }
}

int main() {
  std::string ip = "127.0.0.1";
  int32_t port = 8888;

  server(ip, port);

  return 0;
}