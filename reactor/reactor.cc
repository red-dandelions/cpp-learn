#include "reactor.h"

//#include <glog/logging.h>

namespace reactor {

int32_t Reactor::RegisterEventHandler(std::shared_ptr<Handler> handler) {}

void Reactor::ClearEventHandler(std::shared_ptr<Handler> handler) {}

void Reactor::Run() {
  event_demultiplex_->WaitForEvents(this);
}

int32_t Reactor::Dispatch(std::shared_ptr<Handler> handler) {
  // CHECK(event_demultiplex_) << "event_demultiplex_ is nullptr";

  switch (handler->Type()) {
    case EventType::kAccept: {
      int32_t client = acceptor_->Accept(handler->Fd());
      std::shared_ptr<Handler> handler = std::make_shared<Handler>(client, EventType::kRead);
      RegisterEventHandler(handler);
    } break;
    case EventType::kRead: {
      handler->Read();
    }
      break;
    case EventType::kWrite: {
      handler->Write();
    }
      break;
    default:
      LOG(INFO) << "unknow event type: " << handler->Type();
  }

  return 0;
}

}  // namespace reactor