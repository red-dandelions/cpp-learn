#include "glog/logging.h"

#include <arpa/inet.h>
#include <poll.h>
#include <string_view>
#include <unistd.h>
#include <vector>

void Poll(std::string_view ip, std::string_view port) {
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

  pollfd clients[OPEN_MAX];
  clients[0].fd = socket_fd;
  clients[0].events = POLLRDNORM;
  for (size_t i = 1; i < OPEN_MAX; ++i) {
    clients[i].fd = -1;
  }
  int maxi = 0;  // clients 正在使用的最大下标

  while (true) {
    // 阻塞直到有事件发生
    int nready = poll(clients, maxi + 1, -1);  // ms

    if (clients[0].revents & POLLRDNORM) {
      sockaddr_in client;
      socklen_t len = sizeof(sockaddr_in);
      int conn_fd = accept(socket_fd, (struct sockaddr *)&client, &len);

      int i;
      for (i = 1; i < OPEN_MAX; ++i) {
        if (clients[i].fd < 0) {
          clients[i].fd = conn_fd;
          break;
        }
      }
      CHECK(i < OPEN_MAX) << "too many client";

      clients[i].events = POLLRDNORM;
      maxi = socket_fd > maxi ? socket_fd : maxi;
      if (--nready <= 0) {
        continue;
      }
    }

    // 处理 client 数据
    for (size_t k = 1; k < maxi; ++k) {
      if (clients[k].fd < 0) {
        continue;
      }
      if (clients[k].revents & (POLLRDNORM | POLLERR)) {
        if (true) {
          // 处理完，关闭client socket，清除 fd_set
          close(clients[k].fd);
          clients[k].fd = -1;
        } else if (false) {
          /* TODO */
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