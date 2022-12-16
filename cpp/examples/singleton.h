#pragma once

#include <memory>
#include <mutex>
#include <utility>

namespace examples {

template <typename T>
class SingletonBase {
 public:
  friend class std::unique_ptr<T>;

  static T* Get() { return Instance(); }

  // 创建一个实例
  template <typename... Args>
  static T* Instance(Args&&... args) {
    if (instance_ == nullptr) {
      std::unique_lock<std::mutex> lock(instance_mutex_);
      if (instance_ == nullptr) {
        instance_.reset(new T(std::forward<Args>(args)...));
      }
    }
    return instance_.get();
  }

 protected:
  SingletonBase() {}
  virtual ~SingletonBase() {}

 private:
  static std::mutex instance_mutex_;
  static std::unique_ptr<T> instance_;
};

template <typename T>
std::unique_ptr<T> SingletonBase<T>::instance_;
template <typename T>
std::mutex SingletonBase<T>::instance_mutex_;
template <typename T>
class Singleton : public SingletonBase<T> {};

}  // namespace examples