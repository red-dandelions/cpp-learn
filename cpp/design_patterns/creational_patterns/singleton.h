#pragma once

#include <memory>
#include <mutex>

namespace design_patterns {
namespace creational_patterns {

class Singleton {
 public:
  friend class std::unique_ptr<Singleton>;
  Singleton(Singleton&) = delete;                   // 复制构造函数禁止
  Singleton& operator=(const Singleton&) = delete;  // 赋值运算符禁止

  static Singleton* Instance() {
    if (instance_ == nullptr) {
      std::lock_guard<std::mutex> lock(mutex_);
      if (instance_ == nullptr) {
        instance_ = (new Singleton());
      }
    }
    return instance_;
  }

 protected:
  Singleton() {}
  virtual ~Singleton() {}

 private:
  static Singleton* instance_;
  static std::mutex mutex_;
};

Singleton* Singleton::instance_;
std::mutex Singleton::mutex_;

}  // namespace creational_patterns
}  // namespace design_patterns