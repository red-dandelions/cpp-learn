#pragma once

#include <utility>

namespace tree {

// 小根堆
template <typename T>
class PairTree {
 public:
  template <typename I>
  struct Node {
    I value;
    Node<I>* child;
    Node<I>* brother;
    Node<I>* father;
    Node() : value(I()), child(nullptr), brother(nullptr), father(nullptr) {}
    Node(I v) : value(v), child(nullptr), brother(nullptr), father(nullptr) {}
  };

  PairTree() : root_(nullptr) {}

  ~PairTree() {
    while (root_) {
      Node<T>* node = pop();
      delete node;
    }
  }

  bool empty() { return root_ == nullptr; }

  Node<T>* top() { return root_; }

  void push(Node<T>* node) { root_ = _merge(node, root_); }

  Node<T>* update(Node<T>* node, T value) {
    node->value = value;
    // 如果 node 为根节点，直接返回
    if (node == root_) return root_;
    // 把 node 从 father 的子节点中拆分出去，分 node 的位置讨论
    if (node->father->child == node) {
      node->father->child = node->brother;
    } else {
      node->father->brother = node->brother;
    }

    if (node->brother != nullptr) {
      node->brother->father = node->father;
    }

    node->brother = nullptr;
    node->father = nullptr;

    root_ = _merge(root_, node);
    return root_;
  }

  Node<T>* pop() {
    Node<T>* res = root_;

    root_ = _merges(root_);

    res->brother = nullptr;
    res->father = nullptr;

    return res;
  }

 private:
  Node<T>* _merge(Node<T>* a, Node<T>* b) {
    // 若有一个为空则直接返回另一个
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    if (a->value > b->value) std::swap(a, b);

    if (a->child != nullptr) {
      a->child->father = b;
    }

    b->brother = a->child;
    b->father = a;
    a->child = b;

    return a;
  }

  Node<T>* _merges(Node<T>* root) {
    if (root == nullptr) {
      return root;
    }

    root->father = nullptr;

    if (root->brother == nullptr) {
      return root;
    }

    Node<T>* brother = root->brother;            // root 下一个兄弟
    Node<T>* next_brother = brother->brother;    // brother 下一个兄弟
    brother->father = nullptr;                   // 维护父指针
    root->brother = brother->brother = nullptr;  // 拆散
    return _merge(_merges(next_brother), _merge(root, brother));
  }

  Node<T>* root_;
};

}  // namespace tree