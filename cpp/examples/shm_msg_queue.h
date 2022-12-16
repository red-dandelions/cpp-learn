#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <vector>

namespace examples {

namespace iner {
// 消息队列出队返回状态
enum class ShmMsgQueueResult {
  ok,             // 成功
  timeout,        // 超时
  closed,         // 已关闭
  unknown_error,  // 未知错误
};
// 消息队列存储消息的链表
class ShmMsgChunkList {
 public:
  // 数据结点
  struct Node {
    size_t size;
    const void* data;
    Node* next;
  };

  ShmMsgChunkList();
  ~ShmMsgChunkList();

  void init(int32_t capacity);

  Node* allocate_node();
  void deallocate_node(Node* node);

  void put(Node* node);
  Node* pop();
  Node* nodes();

 private:
  struct Data;
  std::shared_ptr<Data> data_;
};
}  // namespace iner

// 基于共享内存和匿名信号量实现的消息队列，用户进程间高效通信
class SharedMemoryMessageQueue {
 public:
  // 用于初始化消息队列
  struct Options {
    size_t capacity = 128;      // 消息队列的容量
    size_t buffer_size = 64UL;  // 用于通信的共享内存 buffer 大小， 默认为 64M
  };

  using Status = iner::ShmMsgQueueResult;

  explicit SharedMemoryMessageQueue(const std::string& name = "");
  ~SharedMemoryMessageQueue();

  bool init(const Options* options = nullptr);  // 初始化消息队列
  bool allocate(size_t size, bool block, uint32_t* out_chunk_id, void** out_chunk_addr);
  // 消息入队列
  //
  // 如果消息队列的容量不足，当前线程将会被阻塞。直到有线程消费队列中缓存的消息
  void enqueue(uint32_t chunk_id);
  bool enqueue(const void* msg, size_t msg_size);
  // 从消息队列中弹出一个元素，会阻塞当前线程，直到有消息可用
  // 如果队列被关闭，将会立即返回一个 false
  //
  // @param chunk_id 对应消息的 id
  // @param out_addr 地址
  // @param out_chunk_size 对应 chunk 消息的字节数
  Status dequeue(uint32_t* out_chunk_id, const void** out_addr, size_t* out_chunk_size,
                 int64_t timeout_us);
  void release_chunk(uint32_t chunk_id);  // 释放存储消息的 chunk
  void close();                           // 关闭消息队列

  void set_preducer_count(int32_t count);
  void signal_preducing_complete();

 private:
  // 用于管理共享内存的控制块
  struct MmapedControlBlock;

  static std::vector<uint8_t> build_control_block(const Options& options, size_t pool_size,
                                                  void* pool_addr);

  const std::string name_;

  // 多进程共享内存
  //
  // 数据控制块部分
  MmapedControlBlock* control_block_ = nullptr;
  size_t control_block_mmap_size_ = 0;  // 数据控制块共享内存大小
  // 数据部分
  void* buffer_addr_;
  size_t buffer_mmap_size_ = 0;  // buffer 大小

  std::unique_ptr<iner::ShmMsgChunkList> chunk_list_;  // 数据控制块链表
};

}  // namespace examples