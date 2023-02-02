#pragma once

#include "acceptor.h"
#include "event_demultiplex.h"
#include "handler.h"

namespace reactor {

class Reactor {
 public:
  Reactor() : event_demultiplex_(new EpollEventDemultiplex()), acceptor_(new Acceptor()) {}
  Reactor(EventDemultiplex* event_demultiplex)
      : event_demultiplex_(event_demultiplex), acceptor_(new Acceptor()) {}
  ~Reactor(){};

  // 注册事件
  int32_t RegisterEventHandler(std::shared_ptr<Handler> handler);

  // 清除事件
  void ClearEventHandler(std::shared_ptr<Handler> handler);

  // 运行
  void Run();

  // 分发事件
  int32_t Dispatch(std::shared_ptr<Handler> handler);

 private:
  std::unique_ptr<EventDemultiplex> event_demultiplex_;
  std::unique_ptr<Acceptor> acceptor_;
};

}  // namespace reactor