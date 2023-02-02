#include "event_demultiplex.h"
#include "network_utils.h"
#include "reactor.h"

#include <arpa/inet.h>
#include <cstdlib>
#include <glog/logging.h>

#define HOST_IP "127.0.0.1"
#define PORT 8888
#define BACKLOG 128

int main(int argc, char* argv[]) {
  // 检查传递参数
  CHECK(argc <= 2) << "usage: ./reactor [port]";

  // 处理传入的端口并检查是否合理
  int32_t port = argc == 2 ? atoi(argv[1]) : PORT;
  CHECK(port >= 1024) << "port: " << port << ", port should >= 1024";

  // 创建套接字
  int32_t fd = socket(PF_INET, SOCK_STREAM, 0);
  CHECK(fd != -1) << "socker error: " << std::strerror(errno);

  // 设置 fd 为非阻塞
  reactor::SetNonblock(fd);

  // 绑定 ip 和 port
  sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_port = htons(port);
  CHECK(inet_pton(AF_INET, HOST_IP, &server.sin_addr) == 1)
      << "inet_pton failed. error: " << std::strerror(errno);

  CHECK(bind(fd, reinterpret_cast<sockaddr*>(&server), sizeof(server)) != -1)
      << "bind failed, error: " << std::strerror(errno);

  // 监听 fd
  CHECK(listen(fd, BACKLOG) != -1) << "listen failed, error: " << std::strerror(errno);

  LOG(INFO) << "listen on port: " << port;

  std::unique_ptr<reactor::Reactor> reactor = std::make_unique<reactor::Reactor>();

  //
  std::shared_ptr<reactor::EventHandler> event_handle = std::make_shared<reactor::EventHandler>(fd);
  // 注册 accept 事件
  reactor->RegisterEventHandler(event_handle, Event::kEventAccept);

  // 运行
  reactor->Dispatch();

  close(fd);

  return 0;
}