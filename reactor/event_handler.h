#pragma once

#include <cinttypes>

enum class Event { kEventAccept, kEventRead, kEventSend, kEventUnknow };

enum class Op { kAdd, kMod, kDel };

namespace reactor {

class EventHandler {
 public:
  EventHandler(int32_t fd) : fd_(fd), op_(Op::kAdd) {}

  int32_t Fd() { return fd_; }

  Op GetOperator() { return op_; }

  void SetOperator(Op op) { op_ = op; }

  Event GetEventType() { return event_type_; }

  void SetEventType(Event event) { event_type_ = event; }

 private:
  int32_t fd_;
  Op op_;
  Event event_type_;
};

}  // namespace reactor