#include "event_demultiplex.h"

#include <chrono>
#include <glog/logging.h>
#include <sys/epoll.h>

namespace reactor {

EpollEventDemultiplex::EpollEventDemultiplex() { epoll_fd_ = epoll_create(1); }

int32_t EpollEventDemultiplex::RegisterEventHandler(std::shared_ptr<reactor::Handler> handler) {
  if (!handler) {
    return -1;
  }

  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = handler->Fd();
  // 设置为边缘触发
  event.events |= EPOLLET;

  switch (handler->Type()) {
    case EventType::kAccept:
      server_fd_ = handler->Fd();
      event.events |= EPOLLIN;
      LOG(INFO) << "reactor listen socket_fd: " << server_fd_;
      break;
    case EventType::kRead:
      event.events |= EPOLLIN;
      DLOG(INFO) << "add read event";
      break;
    case EventType::kWrite:
      event.events |= EPOLLOUT;
      DLOG(INFO) << "add send event";
      break;
    default:
      LOG(INFO) << "not support event: " << int32_t(handler->Type());
  }

  // 添加 epoll 监听事件
  //
  // 默认为 EPOLL_CTL_ADD
  int32_t op = handler->HasRegistered() ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
  if (epoll_ctl(epoll_fd_, op, handler->Fd(), &event) != 0) {
    LOG(ERROR) << "epoll_ctl failed, error: " << std::strerror(errno);
    return -1;
  }

  // 标记注册
  handler->SetRegistered(true);

  // 关联 fd 和 handler
  fd_to_handler_[handler->Fd()] = handler;

  return 0;
}

void EpollEventDemultiplex::ClearEventHandler(std::shared_ptr<reactor::Handler> handler) {
  if (!handler) {
    return;
  }

  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = handler->Fd();

  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, handler->Fd(), &event) < 0) {
    LOG(ERROR) << "epoll del event faild, fd: " << handler->Fd()
               << " , error: " << std::strerror(errno);
    return;
  }

  fd_to_handler_.erase(handler->Fd());
  DLOG(INFO) << "epoll del success, fd: " << handler->Fd();
}

void EpollEventDemultiplex::WaitForEvents(int32_t timeout) {
  LOG(INFO) << "wait for events";

  struct epoll_event events[MAX_EPOLL_EVENT];

  while (true) {
    int32_t ready = epoll_wait(epoll_fd_, events, MAX_EPOLL_EVENT, timeout);

    for (int32_t i = 0; i < ready; ++i) {
      int32_t ready_fd = events[i].data.fd;

      if (fd_to_handler_.find(ready_fd) != fd_to_handler_.end()) {
        auto handler = fd_to_handler_[ready_fd];
        Dispatch(handler);
      }
    }
  }
}

int32_t EpollEventDemultiplex::Dispatch(std::shared_ptr<Handler> handler) {
  DLOG(INFO) << "dispatch event type: " << int32_t(handler->Type());

  switch (handler->Type()) {
    case EventType::kAccept: {
      // 如果 acceptor 为空则 new 一个
      if (!acceptor_) {
        acceptor_.reset(new Acceptor());
      }
      int32_t client = acceptor_->Accept(handler->Fd());
      std::shared_ptr<Handler> handler = std::make_shared<Handler>(client, EventType::kRead);
      RegisterEventHandler(handler);
    } break;
    case EventType::kRead: {
      handler->Read();
      // 修改监听事件类型，重新注册
      handler->SetType(EventType::kWrite);
      RegisterEventHandler(handler);
    }
      break;
    case EventType::kWrite: {
      handler->Write();
      // 清楚事件
      ClearEventHandler(handler);
    }
      break;
    default:
      LOG(INFO) << "unknow event type: " << int32_t(handler->Type());
  }

  return 0;
}

}  // namespace reactor