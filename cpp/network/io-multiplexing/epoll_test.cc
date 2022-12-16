#include "glog/logging.h"

#include <arpa/inet.h>
#include <poll.h>
#include <string_view>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

void add_fd(int epoll_fd, int fd, bool et) {
  epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN;
  if (et) {
    event.events |= EPOLLET;
  }
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);

  // set nonblocking
  // TODO
}

void LT(epoll_event* events, int num, int epoll_fd, int socket_fd) {  // 水平触发
  char buf[1024];
  for (size_t i = 0; i < num; ++i) {
    int fd = events[i].data.fd;
    if (fd == socket_fd) {
      // 有新 client 连接
      // TODO
      int client_fd;
      add_fd(epoll_fd, client_fd, false);  // 添加 client fd, LT模式
    } else if (events[i].events & EPOLLIN) {
      // 从 fd 读取数据
    } else {
      // 没有定义的事件
    }
  }
}

void ET(epoll_event* events, int num, int epoll_fd, int socket_fd) {  // 边缘触发
  for (size_t i = 0; i < num; ++i) {
    int fd = events[i].data.fd;
    if (fd == socket_fd) {
      // 有新 client 连接
      // TODO
      int client_fd;
      add_fd(epoll_fd, client_fd, true);  // 添加 client fd, ET模式
    } else if (events[i].events & EPOLLIN) {
      // 从 fd 读取数据
      while (true) {
        // 读取完所有数据, 都应该为非阻塞操作
        close(fd);
      }
    } else {
      // 没有定义的事件
    }
  }
}

void Epoll(std::string_view ip, std::string_view port) {
  sockaddr_in server;
  {
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(std::atoi(port.data()));
    inet_pton(AF_INET, ip.data(), &server.sin_addr.s_addr);
  }

  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  CHECK(socket_fd != -1) << "socket error!, error: " << std::strerror(errno);

  CHECK(bind(socket_fd, (const struct sockaddr*)&server, sizeof(server)) != -1) << "bind error!";

  CHECK(listen(socket_fd, 5));

  epoll_event events[1024];
  int epoll_fd = epoll_create(1);  // 这个参数没有意义

  add_fd(epoll_fd, socket_fd);

  while (true) {
    // 阻塞直到有事件发生
    int ret = epoll_wait(epoll_fd, events, 1024, -1);  // 返回就绪数目

    if (ret < 0) return;

    LT(events, ret, epoll_fd, socket_fd);  // 水平触发

    ET(events, ret, epoll_fd, socket_fd);  // 边缘触发
  }

  close(epoll_fd);
}

int main() { return 0; }