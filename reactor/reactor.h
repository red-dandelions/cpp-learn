#pragma once

#include "event_demultiplex.h"
#include "handler.h"

namespace reactor {

class Reactor {
 public:
  Reactor() : event_demultiplex_(new EpollEventDemultiplex()) {}
  Reactor(EventDemultiplex* event_demultiplex)
      : event_demultiplex_(event_demultiplex) {}
  ~Reactor(){};

  // 注册事件
  int32_t RegisterEventHandler(std::shared_ptr<Handler> handler);

  // 清除事件
  void ClearEventHandler(std::shared_ptr<Handler> handler);

  // 运行
  void Run();

 private:
  std::unique_ptr<EventDemultiplex> event_demultiplex_;
};

}  // namespace reactor