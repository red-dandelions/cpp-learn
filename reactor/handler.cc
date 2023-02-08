#include "handler.h"

#include "processor.h"
#include "thread_pool.h"

#include <glog/logging.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace reactor {

std::string TypeToString(EventType type) {
  switch (type) {
    case EventType::kAccept:
      return "kAccept";
    case EventType::kRead:
      return "kRead";
    case EventType::kWrite:
      return "kWrite";
    default:
      return "unknow type";
  }
}

int32_t Handler::Read(std::function<void()> cb) {
  std::string data = buffer_;
  const int32_t SIZE = 1024;
  // 数据接收缓冲区
  char buffer[SIZE + 1];
  // 一次性读完缓冲区
  while (true) {
    memset(buffer, 0, SIZE);
    int32_t bytes = read(fd_, buffer, SIZE);

    if (bytes <= 0) {
      if (errno == EINTR) {
        continue;
      }
      if (bytes == 0 || errno == EAGAIN) {
        break;
      }
      // 出错则设置 error 类型并返回
      LOG(ERROR) << "read failed, error: " << std::strerror(errno);
      SetType(EventType::kError);
      return -1;
    }

    // 处理数据包
    data += buffer;
  }

  // tcp 是流协议，需要逻辑层分包
  //
  // TODO
  request_q_.push(data);
  // data_to_request();

  DLOG(INFO) << "read end";

  // 有数据需要处理
  //
  // 子线程处理完缓冲区中所有的请求再发送响应数据
  if (!request_q_.empty()) {
    // 子线程处理数据
    ThreadPool::Instance()->Run([cb, this]() {
      Processor processor;
      std::lock_guard<std::mutex> lock(this->mutex_);
      while (!this->request_q_.empty()) {
        std::string request = this->request_q_.front();
        request_q_.pop();
        this->response_q_.push(processor.processor());
      }
      cb();
    });
  }
  return 0;
}

int32_t Handler::Write() {
  std::lock_guard<std::mutex> lock(mutex_);

  // CHECK(response_q_.empty() == false) << "message_q_ is empty";

  std::string response = response_q_.front();
  response_q_.pop();

  // TODO
  //
  // 发送数据

  DLOG(INFO) << "message: " << response;

  return 0;
}

int32_t Handler::Error(std::string msg) {
  LOG(ERROR) << "error: " << msg;
  return 0;
}

}  // namespace reactor