#pragma once

#include <vector>

namespace datastructure {
namespace queue {
namespace {
#define SIZE  (1 << 20) // 1 Mb
#define ALIGN_MASK 7
} // namespace

class RingBuffer {
public:
  struct Node {
    size_t size;
    std::vector<uint8_t> data;
  };

  RingBuffer() : capacity_(0), size_(0), buffer_(nullptr), write_(nullptr), read_(nullptr) {}
  ~RingBuffer();

  RingBuffer(const RingBuffer&) = delete;
  RingBuffer& operator=(const RingBuffer&) const = delete;
  RingBuffer(RingBuffer&&) = delete;
  RingBuffer& operator=(RingBuffer&&) const = delete;

  bool init(size_t size=SIZE);
  void push(Node node);
  Node pop();

private:
  //  按 8 字节对齐
  size_t evaluate_aligned_size(size_t size) {
    return (size + ALIGN_MASK) & (~ALIGN_MASK);
  }
  size_t capacity_;
  size_t size_;
  uint8_t* buffer_;
  uint8_t* write_;
  uint8_t* read_;
};

} // namespace queue
} // namespace datastructure