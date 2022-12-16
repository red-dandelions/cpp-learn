/**
 * zero copy。
 * 完全在内核中复制数据，避免在内核和用户缓冲区之间copy数据
 * #include <sys/sendfile.h>
 * ssize_t sendfile(int out_fd, int in_fd, off_t* offset, size_t count);
 * in_fd 必须是真实文件描述符
 * out_fd 必须是 socket
 * error 返回 -1
 */
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
// #include <sys/sendfile.h> // linux 系统
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

void function(std::string ip, std::string port) {}

int main() {
  std::string file_name;
  int filefd = open(file_name.data(), O_RDONLY);
  struct stat stat_buf;
  fstat(filefd, &stat_buf);

  sockaddr_in address;
  int port;
  std::string ip;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip.data(), &address.sin_addr);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);

  bind(sockfd, (struct sockaddr *)&address, sizeof(address));

  listen(sockfd, 5);

  struct sockaddr_in client;
  socklen_t client_addr_len = sizeof(client);
  int connfd = accept(sockfd, (struct sockaddr *)&client, &client_addr_len);

  // linux
  // sendfile(connfd, filefd, nullptr, stat_buf.st_size);

  // mac os
  auto size = stat_buf.st_size;
  sendfile(filefd, sockfd, 0, &size, nullptr, 0);
  close(connfd);
  close(sockfd);
  close(filefd);

  return 0;
}