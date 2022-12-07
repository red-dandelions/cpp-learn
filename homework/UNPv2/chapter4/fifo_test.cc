/**
 * #include <sys/types.h>
 * #include <sys/stat.h>
 * int mkfifo(const char* pathname, mode_t mode);
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstring>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define FIFO1 "/tmp/_parent_to_child.fifo"
#define FIFO2 "/tmp/_child_to_parent.fifo"

// 服务端程序
void server(int read_fd, int write_fd) {
  int32_t n;
  char buf[1025];
  n = read(read_fd, buf, 1024);
  buf[n] = '\0';

  int32_t fd;
  fd = open(buf, O_RDONLY);

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

// 不做错误检查，假定所有系统调用都成功
int main() {
  mkfifo(FIFO1, FILE_MODE);
  mkfifo(FIFO2, FILE_MODE);

  pid_t childpid = fork();
  int read_fd, write_fd;
  // 子进程
  if (childpid == 0) {
    read_fd = open(FIFO1, O_RDONLY, 0);
    write_fd = open(FIFO2, O_WRONLY, 0);
    server(read_fd, write_fd);
    return 0;
  }

  // 父进程
  // 这里互换顺序会造成死锁，在读管道之前没有写入，会一直阻塞
  write_fd = open(FIFO1, O_WRONLY, 0);
  read_fd = open(FIFO2, O_RDONLY, 0);

  client(read_fd, write_fd);

  waitpid(childpid, nullptr, 0);

  close(read_fd);
  close(write_fd);

  unlink(FIFO1);
  unlink(FIFO2);

  return 0;
}