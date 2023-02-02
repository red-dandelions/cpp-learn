#include "reactor.h"

#include <glog/logging.h>

namespace reactor {

int32_t Reactor::RegisterEventHandler(std::shared_ptr<Handler> handler) {
  // 检查 event_demultiplex
  CHECK(event_demultiplex_ != nullptr) << "event_demultiplex is nullptr";
  
  return event_demultiplex_->RegisterEventHandler(handler);
}

void Reactor::ClearEventHandler(std::shared_ptr<Handler> handler) {
  // 检查 event_demultiplex
  CHECK(event_demultiplex_ != nullptr) << "event_demultiplex is nullptr";

  event_demultiplex_->ClearEventHandler(handler);
}

void Reactor::Run() {
  // 检查 event_demultiplex
  CHECK(event_demultiplex_ != nullptr) << "event_demultiplex is nullptr";

  event_demultiplex_->WaitForEvents(); }
}  // namespace reactor