#include <chrono>
#include <iostream>
#include <thread>

void function() {
  std::cout << "this is function, thread id: " << std::this_thread::get_id() << std::endl;
}

void add(int a, int b) {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));  // sleep 10 ms
  std::cout << "a + b = " << (a + b) << " thread id : " << std::this_thread::get_id() << std::endl;
}

int main() {
  // 创建线程
  std::thread thread1(function);
  std::thread thread2(add, 10, 4);

  // 等待线程结束
  thread1.join();
  thread2.join();

  return 0;
}