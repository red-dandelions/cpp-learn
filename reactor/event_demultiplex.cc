#include "event_demultiplex.h"

#include <chrono>
#include <glog/logging.h>
#include <sys/epoll.h>

namespace reactor {

EpollEventDemultiplex::EpollEventDemultiplex() : server_fd_(0), epoll_event_count_(0) {
  epoll_fd_ = epoll_create(1);
  CHECK(epoll_fd_ != -1) << "epoll_create failed. error: " << std::strerror(errno);
}

int32_t EpollEventDemultiplex::RegisterEventHandler(std::shared_ptr<reactor::Handler> handler) {
  if (!handler) {
    LOG(ERROR) << "handler is nullptr";
    return -1;
  }

  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  // 检测对方是否断开链接
  event.events = EPOLLRDHUP;
  event.data.fd = handler->Fd();

  // 设置为边缘触发
  event.events |= EPOLLET;

  switch (handler->Type()) {
    case EventType::kAccept:
      server_fd_ = handler->Fd();
      event.events |= EPOLLIN;
      DLOG(INFO) << "listen socket_fd: " << server_fd_;
      break;
    case EventType::kRead:
      event.events |= EPOLLIN;
      DLOG(INFO) << "add read event";
      break;
    case EventType::kWrite:
      event.events |= EPOLLOUT;
      DLOG(INFO) << "add write event";
      break;
    default:
      LOG(WARNING) << "not support event: " << TypeToString(handler->Type());
  }

  // 添加 epoll 监听事件
  //
  // 默认为 EPOLL_CTL_ADD
  int32_t op = handler->HasRegistered() ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
  if (epoll_ctl(epoll_fd_, op, handler->Fd(), &event) != 0) {
    LOG(ERROR) << "epoll_ctl failed, error: " << std::strerror(errno) << " epoll_fd_: " << epoll_fd_
               << " fd: " << handler->Fd();
    return -1;
  }

  // 增加计数
  ++epoll_event_count_;

  // 标记注册
  handler->SetRegistered(true);

  // 关联 fd 和 handler
  fd_to_handler_[handler->Fd()] = handler;

  DLOG(INFO) << "register event [ fd: " << handler->Fd()
             << " , event type: " << TypeToString(handler->Type()) << " ]";

  return 0;
}

int32_t EpollEventDemultiplex::ClearEventHandler(std::shared_ptr<reactor::Handler> handler) {
  if (!handler) {
    LOG(ERROR) << "handler is nullptr";
    return -1;
  }

  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = handler->Fd();

  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, handler->Fd(), &event) < 0) {
    LOG(ERROR) << "epoll del event faild, fd: " << handler->Fd()
               << " , error: " << std::strerror(errno);
    return -1;
  }

  fd_to_handler_.erase(handler->Fd());

  // 减少计数
  --epoll_event_count_;

  // 关闭 fd
  close(handler->Fd());

  DLOG(INFO) << "clear event [ fd: " << handler->Fd() << " ]";

  return 0;
}

void EpollEventDemultiplex::WaitForEvents(int32_t timeout) {
  LOG(INFO) << "wait for events";

  std::vector<epoll_event> events(epoll_event_count_);

  while (true) {
    int32_t ready = epoll_wait(epoll_fd_, &events[0], epoll_event_count_, timeout);

    CHECK(ready != -1) << "epoll_wait failed, error: " << std::strerror(errno);

    for (int32_t i = 0; i < ready; ++i) {
      int32_t ready_fd = events[i].data.fd;

      // ready_fd 应该一定要存在 map 中
      CHECK(fd_to_handler_.find(ready_fd) != fd_to_handler_.end())
          << "fd: " << ready_fd << " has no handler";

      auto handler = fd_to_handler_[ready_fd];

      // 设置 type 为 kError 交由 Dispatch 处理
      if (events[i].events & EPOLLERR) {
        handler->SetType(EventType::kError);
      }

      // 如果对方已经断开连接，则清除事件
      if (events[i].events & EPOLLRDHUP) {
        LOG(WARNING) << "client [ " << handler->Address() << " ] is disconnected";
        CHECK(ClearEventHandler(handler) == 0) << "clear event failed";
        continue;
      }

      Dispatch(handler);
    }
  }
}

int32_t EpollEventDemultiplex::Dispatch(std::shared_ptr<Handler> handler) {
  DLOG(INFO) << "dispatch event [ fd: " << handler->Fd()
             << " type: " << TypeToString(handler->Type()) << " ]";

  switch (handler->Type()) {
    case EventType::kAccept: {
      // 如果 acceptor 为空则 new 一个
      if (!acceptor_) {
        acceptor_.reset(new Acceptor());
      }
      auto client = acceptor_->Accept(handler->Fd());
      // 设置非阻塞，注册事件
      SetNonblock(client.fd);
      std::shared_ptr<Handler> handler =
          std::make_shared<Handler>(client.fd, EventType::kRead, client.address);
      CHECK(RegisterEventHandler(handler) == 0);
    } break;
    case EventType::kRead: {
      // handler 在子线程处理完数据，通过 cb 通知 handler 注册 kWrite 事件
      auto cb = [this, handler]() {
        handler->SetType(EventType::kWrite);
        this->RegisterEventHandler(handler);
      };
      handler->Read(cb);
    } break;
    case EventType::kWrite:
      handler->Write();
      // 发送完数据重新监听 kRead 事件
      handler->SetType(EventType::kRead);
      CHECK(RegisterEventHandler(handler) == 0);
      break;
    case EventType::kError:
      handler->Error("something wrong");
      // 出错，则清除事件
      CHECK(ClearEventHandler(handler) == 0);
      break;
    default:
      LOG(INFO) << "unknow event type: " << TypeToString(handler->Type());
  }

  return 0;
}

}  // namespace reactor