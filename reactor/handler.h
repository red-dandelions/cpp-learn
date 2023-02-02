#pragma once

#include <cinttypes>

enum class EventType {
  kAccept,
  kRead,
  kWrite,
};

namespace reactor {

class Handler {
 public:
  Handler(int32_t fd, EventType type) : fd_(fd), event_type_(type), registered_(false) {}
  ~Handler() {}

  int32_t Read();

  int32_t Write();

  int32_t Fd() { return fd_; }

  EventType Type() { return event_type_; }

  void SetType(EventType type) { event_type_ = type; }

  // 如果已经注册了返回 true
  bool HasRegistered() { return registered_; }

  //
  void SetRegistered(bool flag) { registered_ = flag; }

 private:
  int32_t fd_;
  // 事件的类型
  EventType event_type_;
  // 是否注册
  bool registered_;
};

}  // namespace reactor