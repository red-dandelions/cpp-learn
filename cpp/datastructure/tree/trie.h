#pragma once

#include <string>
#include <unordered_map>

namespace tree {
namespace {
struct Node {
  bool end;
  std::unordered_map<char, Node*> children;
  Node() : end(false) {}
};
}  // namespace

class Trie {
 public:
  Trie() : root_(new Node()) {}
  ~Trie() {
    std::function<void(Node*)> _destroy;
    _destroy = [&](Node* node) {
      if (node == nullptr) return;
      for (auto p : node->children) {
        _destroy(p.second);
      }
      delete node;
    };

    _destroy(root_);
  }

  bool insert(std::string s) {
    Node* tmp = root_;
    for (char ch : s) {
      if (tmp->children.find(ch) == tmp->children.end()) {
        tmp->children[ch] = new Node();
      }
      tmp = tmp->children[ch];
    }
    tmp->end = true;
    return true;
  }

  bool query(std::string s) {
    Node* tmp = root_;
    for (char ch : s) {
      if (tmp->children.find(ch) != tmp->children.end()) {
        tmp = tmp->children[ch];
      } else {
        return false;
      }
    }
    return tmp->end;
  }

  bool start_with(std::string s) {
    Node* tmp = root_;
    for (char ch : s) {
      if (tmp->children.find(ch) != tmp->children.end()) {
        tmp = tmp->children[ch];
      } else {
        return false;
      }
    }
    return true;
  }

 private:
  Node* root_;
};

}  // namespace tree