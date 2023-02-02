#include "event_demultiplex.h"

#include <glog/logging.h>

namespace reactor {

EpollEventDemultiplex::EpollEventDemultiplex() { epoll_fd_ = epoll_create(1); }

int32_t EpollEventDemultiplex::RegisterEventHandler(
    std::shared_ptr<reactor::EventHandler> event_handler, Event event) {
  if (!event_handler) {
    return -1;
  }

  epoll_event event_item;
  memset(&event_item, 0, sizeof(event_item));
  event_item.data.fd = event_handler->Fd();
  // 设置为边缘触发
  event_item.events |= EPOLLET;

  switch (event) {
    case Event::kEventAccept:
      server_fd_ = event_handler->Fd();
      event_item.events |= EPOLLIN;
      LOG(INFO) << "reactor listen socket_fd: " << server_fd_;
      break;
    case Event::kEventRead:
      event_item.events |= EPOLLIN;
      DLOG(INFO) << "add read event";
      break;
    case Event::kEventSend:
      event_item.events |= EPOLLOUT;
      DLOG(INFO) << "add send event";
      break;
    default:
      LOG(INFO) << "not support event: " << int32_t(event);
  }

  // 添加 epoll 监听事件
  //
  // 默认为 EPOLL_CTL_ADD
  int32_t op = int32_t(event_handler->GetOperator());
  if (epoll_ctl(epoll_fd_, op, event_handler->fd(), &event_item) != 0) {
    LOG(ERROR) << "epoll_ctl failed, error: " << std::strerror(errno);
    return -1;
  }

  return 0;
}

void EpollEventDemultiplex::ClearEventHandler(
    std::shared_ptr<reactor::EventHandler> event_handler) {
  if (!event_handler) {
    return;
  }

  epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = event_handler->Fd();

  int32_t op = EPOLL_CTL_DEL;
  if (epoll_ctl(epoll_fd_, op, event_handler->Fd(), &event) < 0) {
    LOG(ERROR) << "epoll del event faild, fd: " << event_handler->Fd()
               << " , error: " << std::strerror(errno);
    return;
  }

  DLOG(INFO) << "epoll del success, fd: ";
  event_handler->Fd();
  return;
}

int32_t EpollEventDemultiplex::WaitForEvents(int32_t timeout) {
  struct epoll_events events[MAX_EPOLL_EVENT];

  int32_t ready = epoll_wait(epoll_fd_, events, MAX_EPOLL_EVENT, timeout);

  for (int32_t i = 0; i < ready; ++i) {
  }
}

}  // namespace reactor