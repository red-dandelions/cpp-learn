#include <condition_variable>
#include <iostream>
#include <thread>

class Foo {
 public:
  Foo() : flag_(false) {}
  Foo(const Foo&) = delete;

  void Run() {
    std::unique_lock<std::mutex> lock(mutex_);
    std::cout << "wait for 1 s\n";
    cv_.wait_for(lock, std::chrono::seconds(1));
    std::cout << "wait end. wait for flag is true, now flag: " << flag_ << std::endl;
    // 只有 flag_ 为 true 时才能被唤醒
    cv_.wait(lock, [this] { return this->flag_; });
    std::cout << "now flag: " << flag_ << std::endl;
  }

  void SetFlag(bool flag) { flag_ = flag; }

  void Notify(bool flag) {
    std::cout << "try to notify Run\n";
    SetFlag(flag);
    cv_.notify_one();
  }

  bool Flag() { return flag_; }

 private:
  bool flag_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

void function(Foo* a) { a->Run(); }

int main() {
  Foo a;
  // 创建线程
  std::thread thread_func(function, &a);
  std::this_thread::sleep_for(std::chrono::seconds(3));

  // 尝试唤醒
  a.Notify(false);
  std::this_thread::sleep_for(std::chrono::seconds(3));
  a.Notify(true);

  thread_func.join();

  return 0;
}