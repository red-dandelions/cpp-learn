#include "glog/logging.h"

#include <arpa/inet.h>
#include <string_view>
#include <sys/select.h>
#include <unistd.h>
#include <vector>

void Select(std::string_view ip, std::string_view port) {
  sockaddr_in server;
  {
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(std::atoi(port.data()));
    inet_pton(AF_INET, ip.data(), &server.sin_addr.s_addr);
  }

  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  CHECK(socket_fd != -1) << "socket error!, error: " << std::strerror(errno);

  CHECK(bind(socket_fd, (const struct sockaddr *)&server, sizeof(server)) != -1) << "bind error!";

  CHECK(listen(socket_fd, 5));

  fd_set rset, allset;
  FD_ZERO(&allset);
  FD_SET(socket_fd, &allset);

  std::vector<int> clients(FD_SETSIZE, -1);
  int maxfd = socket_fd;
  while (true) {
    rset = allset;
    // 阻塞直到有事件发生
    int nready = select(maxfd, &rset, nullptr, nullptr, nullptr);

    int max_client = -1;
    if (FD_ISSET(socket_fd, &rset)) {
      sockaddr_in client;
      socklen_t len = sizeof(sockaddr_in);
      int conn_fd = accept(socket_fd, (struct sockaddr *)&client, &len);

      int i;
      for (i = 0; i < FD_SETSIZE; ++i) {
        if (clients[i] < 0) {
          clients[i] = conn_fd;
          break;
        }
      }
      CHECK(i < FD_SETSIZE) << "too many client";

      FD_SET(conn_fd, &allset);
      maxfd = socket_fd > maxfd ? socket_fd : maxfd;
      if (--nready <= 0) {
        continue;
      }
      max_client = i > max_client ? i : max_client;
    }

    // 处理 client 数据
    for (size_t k = 0; k < max_client; ++k) {
      if (clients[k] < 0) {
        continue;
      }
      if (FD_ISSET(clients[k], &rset)) {
        if (true) {
          // 处理完，关闭client socket，清除 fd_set
          close(clients[k]);
          FD_CLR(clients[k], &allset);
          clients[k] = -1;
        } else {
          /* TODO */
        }

        if (--nready <= 0) {
          break;  // 没有事件处理了
        }
      }
    }
  }
}

int main() { return 0; }