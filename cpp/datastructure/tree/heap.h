#include <iostream>
#include <vector>

namespace tree {
// 大根堆
template <typename T>
class MaxHeap {
 public:
  MaxHeap() : size_(0) {}

  bool empty() { return size_ == 0; }

  size_t size() { return size_; }

  T top() { return buffer_[0]; }

  void push(const T value) {
    ++size_;
    if (size_ > buffer_.size()) {
      buffer_.push_back(value);
    } else {
      buffer_[size_ - 1] = value;
    }
    // 从下向上调整大根堆
    auto _heapify = [&]() -> void {
      size_t child = size_ - 1;
      while (child) {
        size_t parent = (child - 1) / 2;
        if (buffer_[parent] < buffer_[child]) {
          std::swap(buffer_[parent], buffer_[child]);
          child = parent;
          continue;
        }
        break;
      }
    };

    _heapify();
  }

  void pop() {
    std::swap(buffer_[0], buffer_[size_ - 1]);
    --size_;
    // 从上向下调整大根堆
    auto _heapify = [&]() -> void {
      size_t parent = 0;
      size_t child = 2 * parent + 1;
      while (child < size_) {
        if (child + 1 < size_ && buffer_[child] < buffer_[child + 1]) {
          child++;
        }
        if (buffer_[parent] < buffer_[child]) {
          std::swap(buffer_[parent], buffer_[child]);
          parent = child;
          child = 2 * parent + 1;
          continue;
        }
        break;
      }
    };

    _heapify();
  }

 private:
  size_t size_;
  std::vector<T> buffer_;
};

// 小根堆
template <typename T>
class MinHeap {
 public:
  MinHeap() : size_(0) {}

  bool empty() { return size_ == 0; }

  size_t size() { return size_; }

  T top() { return buffer_[0]; }

  void push(const T value) {
    ++size_;
    if (size_ > buffer_.size()) {
      buffer_.push_back(value);
    } else {
      buffer_[size_ - 1] = value;
    }
    // 从下向上调整小根堆
    auto _heapify = [&]() -> void {
      size_t child = size_ - 1;
      while (child) {
        size_t parent = (child - 1) / 2;
        if (buffer_[parent] > buffer_[child]) {
          std::swap(buffer_[parent], buffer_[child]);
          child = parent;
          continue;
        }
        break;
      }
    };

    _heapify();
  }

  void pop() {
    std::swap(buffer_[0], buffer_[size_ - 1]);
    --size_;
    // 从上向下调整小根堆
    auto _heapify = [&]() -> void {
      size_t parent = 0;
      size_t child = 2 * parent + 1;
      while (child < size_) {
        if (child + 1 < size_ && buffer_[child] > buffer_[child + 1]) {
          child++;
        }
        if (buffer_[parent] > buffer_[child]) {
          std::swap(buffer_[parent], buffer_[child]);
          parent = child;
          child = 2 * parent + 1;
          continue;
        }
        break;
      }
    };

    _heapify();
  }

 private:
  size_t size_;
  std::vector<T> buffer_;
};

}  // namespace tree