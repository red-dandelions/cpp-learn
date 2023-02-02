#pragma once

#include "event_handler.h"

#include <cinttypes>
#include <memory>

namespace reactor {
class EventDemultiplex {
 public:
  virtual ~EventDemultiplex() {}

  virtual int32_t RegisterEventHandler(std::shared_ptr<reactor::EventHandler> event_handler,
                                       Event event) = 0;
  virtual void ClearEventHandler(std::shared_ptr<reactor::EventHandler> event_handler) = 0;

  virtual Event WaitForEvents(int32_t timeout = -1) = 0;
};

class SelectEventDemultiplex : public EventDemultiplex {};

class PollEventDemultiplex : public EventDemultiplex {};

class EpollEventDemultiplex : public EventDemultiplex {
 public:
  EpollEventDemultiplex();

  virtual int32_t RegisterEventHandler(std::shared_ptr<reactor::EventHandler> event_handler,
                                       Event event);
  virtual void ClearEventHandler(std::shared_ptr<reactor::EventHandler> event_handler);

  virtual Event WaitForEvents(int32_t timeout = -1);

 private:
  // epoll 的文件描述符
  int32_t epoll_fd_;
  // 处理 accept 的文件描述符
  int32_t server_fd_;

  const int32_t MAX_EPOLL_EVENT = 1024 * 1024;
};

}  // namespace reactor