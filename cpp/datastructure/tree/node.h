#pragma once

namespace tree {

template <typename T>
struct Node {
  T value;
  Node<T>* left;
  Node<T>* right;
  Node<T>(T v = T()) : value(v), left(nullptr), right(nullptr) {}
};

}  // namespace tree