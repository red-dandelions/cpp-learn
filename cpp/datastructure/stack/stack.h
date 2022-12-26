#pragma once

#include <cstddef>

namespace stack {
namespace {
template <typename T>
struct Node {
  T value;
  Node<T>* next;
  Node(T v) : value(v), next(nullptr) {}
};
}  // namespace

template <typename T>
class Stack {
 public:
  Stack() : size_(0), tail_(nullptr) {}
  ~Stack() {
    while (tail_) {
      auto tmp = tail_;
      tail_ = tail_->next;
      delete tmp;
    }
  }

  bool empty() { return tail_ == nullptr; }

  size_t size() { return size_; }

  T top() { return tail_->value; }

  void push(T value) {
    Node<T>* node = new Node<T>(value);
    if (empty()) {
      tail_ = node;
    } else {
      node->next = tail_;
      tail_ = node;
    }
    ++size_;
  }

  T pop() {
    if (empty()) {
      return T(-1);
    }
    Node<T>* node = tail_;
    tail_ = tail_->next;
    T value = node->value;
    delete node;
    --size_;
    return value;
  }

 private:
  size_t size_;
  Node<T>* tail_;
};

}  // namespace stack