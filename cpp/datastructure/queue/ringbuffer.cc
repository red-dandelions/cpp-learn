#include "ringbuffer.h"

#include "glog/logging.h"

#include <cstring>
#include <pthread.h>
#include <sys/mman.h>

namespace datastructure {
namespace queue {
namespace {
struct MutexLocker {
  explicit MutexLocker(pthread_mutex_t* mutex) : mutex_(mutex) {
    CHECK(pthread_mutex_lock(mutex_) == 0) << " err_msg: " << strerror(errno);
  }
  ~MutexLocker() { CHECK(pthread_mutex_unlock(mutex_) == 0) << " err_msg: " << strerror(errno); }
  pthread_mutex_t* mutex_;
};
}  // namespace

bool RingBuffer::init(size_t size) {
  capacity_ = evaluate_aligned_size(size);
  buffer_ = reinterpret_cast<uint8_t*>(
      mmap(nullptr, capacity_, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
  if (buffer_ == MAP_FAILED) {
    LOG(ERROR) << "mmap failed, error: " << std::strerror(errno) << ", mmap_size: " << capacity_;
    return false;
  }
  write_ = nullptr;
  read_ = nullptr;

  // init mutex
  pthread_mutexattr_t mutex_attr = {};
  CHECK(pthread_mutexattr_init(&mutex_attr) == 0) << " err_msg: " << strerror(errno);
  CHECK(pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED) == 0)
      << " err_msg: " << strerror(errno);
  CHECK(pthread_mutex_init(&mutex_, &mutex_attr) == 0) << " err_msg: " << strerror(errno);
  CHECK(pthread_mutexattr_destroy(&mutex_attr) == 0) << " err_msg: " << strerror(errno);

  return true;
}

RingBuffer::~RingBuffer() {
  if (buffer_) {
    munmap(buffer_, capacity_);
  }
}

void RingBuffer::push(Node node) {
  MutexLocker lock(&mutex_);
  if (empty()) {
    read_ = buffer_;
    write_ = buffer_;
  }
  size_t data_size = evaluate_aligned_size(node.size);
  size_t size = sizeof(node.size) + data_size;
  if (size > capacity_) {
    LOG(FATAL) << "node size > capacity_, node size: " << size << " , capacity_: " << capacity_;
    return;
  }

  while (capacity_ - (((write_ - read_) + capacity_) % capacity_) < size) {
    size_t node_data_size = *reinterpret_cast<size_t*>(read_);
    size_t offset = 8 + evaluate_aligned_size(node_data_size);

    size_t left_size = capacity_ - (read_ - buffer_);

    if (left_size < offset) {
      read_ = buffer_ + offset - left_size;
    } else {
      read_ += offset;
    }

    if (read_ - buffer_ == capacity_) {
      read_ = buffer_;
    }

    if (read_ == write_) {
      read_ = buffer_;
      write_ = buffer_;
    }
  }

  // 填写 data size
  *reinterpret_cast<size_t*>(write_) = node.size;
  write_ += 8;

  //  距 buffer_ 尾部可写 size
  size_t free_size = capacity_ - (write_ - buffer_);
  if (free_size < data_size) {
    std::memcpy(write_, node.data.data(), free_size);
    std::memcpy(buffer_, node.data.data() + free_size, data_size - free_size);
    write_ = buffer_ + data_size - free_size;
  } else {
    std::memcpy(write_, node.data.data(), data_size);
    write_ += data_size;
  }

  if (write_ - buffer_ == capacity_) {
    write_ = buffer_;
  }
}

RingBuffer::Node RingBuffer::pop() {
  MutexLocker lock(&mutex_);
  if (empty()) {
    LOG(WARNING) << "RingBuffer has no data.";
    return Node{.size = 0, .data = std::vector<uint8_t>()};
  }
  // 获取 data size
  size_t size = *reinterpret_cast<size_t*>(read_);
  size_t data_size = evaluate_aligned_size(size);
  read_ += 8;

  // 数据部分相对于 buffer_ 尾部偏移量
  size_t tail_offset = capacity_ - (read_ - buffer_);

  Node node{.size = size, .data = std::vector<uint8_t>(data_size, 0)};

  if (tail_offset < data_size) {
    std::memcpy(node.data.data(), read_, tail_offset);
    std::memcpy(node.data.data() + tail_offset, buffer_, data_size - tail_offset);
    read_ = buffer_ + data_size - tail_offset;
  } else {
    std::memcpy(node.data.data(), read_, data_size);
    read_ += data_size;
  }

  node.data.resize(size);

  if (read_ - buffer_ == capacity_) {
    read_ = buffer_;
  }

  // 没有数据将指针置为空
  if (read_ == write_) {
    read_ = nullptr;
    write_ = nullptr;
  }

  return node;
}

}  // namespace queue
}  // namespace datastructure