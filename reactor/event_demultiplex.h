#pragma once

#include "acceptor.h"
#include "handler.h"
#include "network_utils.h"

#include <cinttypes>
#include <memory>
#include <unordered_map>

namespace reactor {

class EventDemultiplex {
 public:
  virtual ~EventDemultiplex() {}

  virtual int32_t RegisterEventHandler(std::shared_ptr<Handler> handler) = 0;

  virtual int32_t ClearEventHandler(std::shared_ptr<Handler> handler) = 0;

  virtual void WaitForEvents(int32_t timeout = -1) = 0;
};

class SelectEventDemultiplex : public EventDemultiplex {};

class PollEventDemultiplex : public EventDemultiplex {};

class EpollEventDemultiplex : public EventDemultiplex {
 public:
  EpollEventDemultiplex();
  virtual ~EpollEventDemultiplex() {}

  int32_t RegisterEventHandler(std::shared_ptr<Handler> handler) override;

  int32_t ClearEventHandler(std::shared_ptr<Handler> handler) override;

  void WaitForEvents(int32_t timeout = -1) override;

  int32_t Dispatch(std::shared_ptr<Handler> handler);

 private:
  // epoll 的文件描述符
  int32_t epoll_fd_;
  // 处理 accept 的文件描述符
  int32_t server_fd_;
  // 处理 accept 请求;
  std::unique_ptr<Acceptor> acceptor_;

  // epoll 注册事件数
  int32_t epoll_event_count_;

  // 文件描述符 fd 到 Handler 的映射
  std::unordered_map<int32_t, std::shared_ptr<Handler>> fd_to_handler_;
};

}  // namespace reactor