#include <cstring>
#include <signal.h>

void sig_handler(int sig) {}

int main() {
  int sig = SIGHUP;
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  // sa_mask
  // 成员用来指定在信号处理函数执行期间需要被屏蔽的信号，特别是当某个信号被处理时，它自身会被
  // 自动放入进程的信号掩码，因此在信号处理函数执行期间这个信号不会再度发生。
  sigfillset(&sa.sa_mask);
  sa.sa_flags |= SA_RESTART;
  sa.sa_handler = sig_handler;

  sigaction(sig, &sa, nullptr);

  return 0;
}
