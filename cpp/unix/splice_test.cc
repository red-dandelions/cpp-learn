// mac os 没有
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  // echo: zero copy impl
  std::string ip;
  int port;

  struct sockaddr_in address;
  std::memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(PF_INET, ip.data(), &address.sin_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
  listen(sockfd, 5);

  struct sockaddr_in client;
  socklen_t client_len = sizeof(client);
  int connfd = accept(sockfd, (struct sockaddr *)&client, &client_len);

  int pipefd[2];
  pipe(pipefd);

  // linux
  // ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
  // ret = splice(pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
  close(connfd);

  return 0;
}