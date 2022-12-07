#include <fcntl.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

#include "glog/logging.h"

// 服务端程序
void server(int read_fd, int write_fd) {
  int32_t n;
  char buf[1025];
  n = read(read_fd, buf, 1024);
  buf[n] = '\0';

  int32_t fd;
  fd = open(buf, O_RDONLY);
  CHECK(fd != -1) << "open error: " << std::strerror(errno);

  while ((n = read(fd, buf, 1024)) > 0) {
    write(write_fd, buf, n);
  }

  close(fd);
}

// 客户端程序
void client(int read_fd, int write_fd) {
  size_t len;
  char buf[1024];
  fgets(buf, 1024, stdin);
  len = strlen(buf);
  // 去掉 \n 换行
  if (buf[len - 1] == '\n') {
    --len;
  }

  // 向服务端写入数据
  write(write_fd, buf, len);

  // 从服务端读入数据
  int32_t n;
  while ((n = read(read_fd, buf, 1024)) > 0) {
    write(STDOUT_FILENO, buf, n);  // 输出到 stdout
  }
}

int main() {
  int pfd1[2];  // 用于父进程向子进程传递数据
  int pfd2[2];  // 用于子进程向父进程传递数据

  // 创建管道
  CHECK(pipe(pfd1) != -1) << "creat pfd1 error: " << std::strerror(errno);
  CHECK(pipe(pfd2) != -1) << "creat pfd2 error: " << std::strerror(errno);

  // 创建子进程
  pid_t childpid;
  if ((childpid = fork()) == 0) {
    // 子进程
    close(pfd1[1]);  // pfd1 关闭向父进程写
    close(pfd2[0]);  // pfd2 关闭从父进程读

    server(pfd1[0], pfd2[1]);  // 运行服务程序

    _exit(0);
  }

  // 父进程
  close(pfd1[0]);  // pfd1 关闭从子进程读
  close(pfd2[1]);  // pfd2 关闭向子进程写

  client(pfd2[0], pfd1[1]);  // 运行客户程序

  waitpid(childpid, NULL, 0);  // 等待子进程结束

  return 0;
}