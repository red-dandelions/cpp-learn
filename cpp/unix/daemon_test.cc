#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <unistd.h>
void daemonize() {
  int i, fd0, fd1, fd2, ret;
  pid_t pid;
  struct rlimit rl;
  struct sigaction sa;

  // mask
  umask(0);

  // 获取最大文件描述符
  ret = getrlimit(RLIMIT_NOFILE, &rl);
  if (ret < -1) {
    std::abort();
  }

  // 确保不是首进程
  pid = fork();
  if (pid < 0) {
    std::abort();
  }

  if (pid != 0) {
    // 父进程退出
    std::exit(0);
  }

  // 设置会话
  // 成为会话 leader 从而脱离 TTY
  // 成为进程组首进程
  setsid();

  /*
   * Ensure future opens won't allocate controlling TTYs.
   */
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  ret = sigaction(SIGHUP, &sa, nullptr);
  if (ret < 0) {
    std::abort();
  }

  // 不是会话首领，没有机会分配到一个控制终端
  pid = fork();
  if (pid < 0) {
    std::abort();
  }

  if (pid != 0) {
    // 父进程退出
    std::exit(0);
  }

  // 更改目录
  chdir("/");

  // 关闭所有打开的文件描述符
  if (rl.rlim_max == RLIM_INFINITY) {
    rl.rlim_max = 1024;
  }
  for (i = 0; i < rl.rlim_max; ++i) {
    close(i);
  }

  // 重定向 0， 1， 2 -> /dev/null
  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  // 初始化 log file
  // TODO
}

int main() {
  daemonize();

  sleep(20);

  return 0;
}