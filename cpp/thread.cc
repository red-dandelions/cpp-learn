#include <thread>
#include <iostream>
#include <chrono>

void function() {
  std::cout << "this is function, thread id: " << std::this_thread::get_id() << std::endl;
}
void add(int a, int b) {
  std::cout << "a + b = " << (a + b) << " thread id : " << std::this_thread::get_id() << std::endl;
}
int main() {

  std::thread thread1(function);
  std::thread thread2(add, 10, 4);

  thread1.join(); // 等待线程结束

  thread2.join();

  return 0;
}