#pragma once

#include <cinttypes>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <unistd.h>

enum class EventType { kAccept, kRead, kWrite, kError };

namespace reactor {
// 将 type 转换为字符串
std::string TypeToString(EventType type);

class Handler {
 public:
  Handler(int32_t fd, EventType type, std::string address = "")
      : fd_(fd), address_(address), event_type_(type), registered_(false) {}
  ~Handler() { close(fd_); }

  int32_t Read(std::function<void()>);

  int32_t Write();

  int32_t Error(std::string msg);

  int32_t Fd() { return fd_; }

  std::string Address() { return address_; }

  EventType Type() { return event_type_; }

  void SetType(EventType type) { event_type_ = type; }

  // 如果已经注册了返回 true
  bool HasRegistered() { return registered_; }

  //
  void SetRegistered(bool flag) { registered_ = flag; }

 private:
  int32_t fd_;
  std::string address_;
  // 事件的类型
  EventType event_type_;
  // 是否注册
  bool registered_;
  // 数据缓冲区
  std::string buffer_;
  // 请求队列
  std::queue<std::string> request_q_;
  // 响应队列
  std::queue<std::string> response_q_;
  // 互斥锁
  std::mutex mutex_;
};

}  // namespace reactor