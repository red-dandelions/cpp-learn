#include "node.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace tree {
// 大根堆
template <typename T>
class MaxHeap {
 public:
  bool empty() { return root_ == nullptr; }
  size_t size() { return size_; }

  T top() { return empty() ? T() : root_->value; }

  void push(const T value) {
    auto node = std::make_shared<Node<T>>(value);

    ++size_;
  }
  void pop();

 private:
  size_t size_;
  std::shared_ptr<Node<T>> root_;
  std::shared_ptr<Node<T>> tail_;
};

// 小根堆
template <typename T>
class MinHeap {
 public:
  bool empty() { return root_ == nullptr; }
  size_t size() { return size_; };

  T top();
  void push(const T value) { ++size_; }
  void pop() { --size_; }

 private:
  size_t size_;
  std::shared_ptr<Node<T>> root_;
  std::shared_ptr<Node<T>> tail_;
};

}  // namespace tree