#ifndef __UNPV2_MQUEUE_H__
#define __UNPV2_MQUEUE_H__

#include <cinttypes>
#include <csignal>
#include <pthread.h>
#include <unistd.h>

namespace unpv2 {

// 队列的属性
struct mq_attr {
  int64_t mq_flags;    // mq flag: O_NONBLOCK
  int64_t mq_maxmsg;   // 队列的容量
  int64_t mq_msgsize;  // 消息能携带的最大数据
  int64_t mq_curmsgs;  // 当前队列的消息数
};

// 队列头部信息
struct mq_hdr {
  mq_attr mqh_attr;     // 队列的属性
  int64_t mqh_head;     // 第一个存放的消息，距离头部偏移量为 mqh_head 个字节
  int64_t mqh_free;     // 第一个空闲的消息，距离头部偏移量为 mqh_free 个字节
  int64_t mqh_nwait;    // 有 mqh_nwait 个消费者等待消息
  pid_t mqh_pid;        // 设置了信号通知时要设置
  sigevent* mqh_event;  // 信号通知
  pthread_mutex_t mqh_lock;  // 互斥量
  pthread_cond_t mqh_wait;   // 条件变量
};

// 消息的头部信息
struct msg_hdr {
  int64_t msg_next;   // 下一个消息的偏移量
  int64_t msg_len;    // 消息数据的长度
  uint64_t msg_prio;  // 优先级
};

struct mq_info {
  mq_hdr* mqi_hdr;    // 指向映射文件地址
  int64_t mqi_magic;  // MQI_MAGIC
  int mqi_flags;      // 当前队列本次打开实例的非阻塞标志
};

typedef struct mq_info* mqd_t;

#define MQI_MAGIC 0x98765432
#define IMASK (sizeof(int64_t) - 1)
#define MSGSIZE(i) ((i + IMASK) & ~(IMASK))  // 对齐

mqd_t mq_open(const char* pathname, int oflag, ...);
int mq_close(mqd_t mqd);
int mq_unlink(const char* pathname);
int mq_getattr(mqd_t mqd, mq_attr* mqstat);
int mq_setattr(mqd_t mqd, const mq_attr* mqstat, mq_attr* omqstat);
int mq_notify(mqd_t mqd, const sigevent* notification);
int mq_send(mqd_t mqd, const char* ptr, size_t len, unsigned int prio);
int mq_receive(mqd_t mqd, char* ptr, size_t maxlen, unsigned int* priop);

}  // namespace unpv2

#endif