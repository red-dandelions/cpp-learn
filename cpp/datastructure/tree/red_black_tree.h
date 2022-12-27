#pragma once

/**
 * 红黑树性质：
 * 1. 每个节点是红色或者黑色
 * 2. 根节点是黑色的
 * 3. 每个叶子节点(null_node)是黑色的
 * 4. 如果一个节点是红色的，则它的两个子节点都是黑色的
 * 5. 对于每个节点，从该节点到所有后代叶节点的简单路径上，黑色节点数目相同
 *
 * 红黑树节点定义：
 * 1. 左孩子
 * 2. 右孩子
 * 3. 父节点
 * 4. key值
 * 5. 颜色值
 * 6. value值（根据需求，可选）
 */

#include <functional>

namespace tree {
enum class COLOR { RED, BLACK };

template <typename T, typename I>
struct Node {
  T key;
  I value;
  COLOR color;
  Node() : left(nullptr), right(nullptr), parent(nullptr) {}
  Node *left;
  Node *right;
  Node *parent;
};

template <typename T, typename I>
class RBTree {
 public:
  RBTree() : null_node_(new Node<T, I>()), root_(null_node_) {
    null_node_->left = root_;
    null_node_->right = root_;
    null_node_->parent = root_;
    null_node_->color = COLOR::BLACK;
  }

  // 递归 delete 节点
  ~RBTree() {
    std::function<void(Node<T, I> *)> _destroy;
    _destroy = [&](Node<T, I> *node) {
      if (node == null_node_) {
        return;
      }
      _destroy(node->left);
      _destroy(node->right);
      delete node;
    };
    _destroy(root_);
    delete null_node_;
  }

  // 禁用复制构造函数和赋值运算符
  RBTree(const RBTree &) = delete;
  RBTree &operator=(const RBTree &) = delete;

  bool empty() { return null_node_ == root_; }

  Node<T, I> *find(T key) {
    Node<T, I> *node = root_;
    while (node != null_node_) {
      if (node->key == key) {
        return node;
      } else if (node->key < key) {
        node = node->right;
      } else {
        node = node->left;
      }
    }
    return nullptr;
  }

  void insert(T key, I value);

  bool erase(T key);

 private:
  void _insert_fixup(Node<T, I> *node);
  void _erase_fixup(Node<T, I> *node);
  void _rotate_left(Node<T, I> *node);
  void _rotate_right(Node<T, I> *node);

  Node<T, I> *null_node_;  // 所有叶子结点指向的空节点
  Node<T, I> *root_;       // 根节点

};  // RBTree

// 插入函数，插入位置肯定先是是叶子节点，之后再调整
template <typename T, typename I>
void RBTree<T, I>::insert(T key, I value) {
  Node<T, I> *iter_node = root_;                // 用来遍历，查找插入节点的位置
  Node<T, I> *insert_parent_node = null_node_;  // 插入节点的父节点

  // 找到插入节点的父节点
  while (iter_node != null_node_) {
    insert_parent_node = iter_node;
    if (key < iter_node->key) {
      iter_node = iter_node->left;
    } else if (key > iter_node->key) {
      iter_node = iter_node->right;
    } else {
      // 已经存在 key 了，先删除 key，再插入
      erase(key);
      insert(key, value);
      return;
    }
  }

  // 生成插入节点，颜色为黑，左右孩子都是 null_node_
  Node<T, I> *insert_node = new Node<T, I>();
  insert_node->parent = insert_parent_node;
  insert_node->key = key;
  insert_node->value = value;
  insert_node->color = COLOR::RED;
  insert_node->left = null_node_;
  insert_node->right = null_node_;

  // 判断插入节点在哪个位置, key 不会相等，因为相等会删除后再插入
  if (insert_parent_node == null_node_) {
    // 树为空时，根节点就为插入节点
    root_ = insert_node;
  } else if (insert_node->key < insert_parent_node->key) {
    // 作为左孩子
    insert_parent_node->left = insert_node;
  } else {
    // 作为右孩子
    insert_parent_node->right = insert_node;
  }

  // 调用辅助函数重新着色并旋转
  _insert_fixup(insert_node);
}

template <typename T, typename I>
bool RBTree<T, I>::erase(T key) {
  // 首先判断是否存在 key
  auto erase_node = find(key);
  if (erase_node == nullptr) {
    return false;
  }

  // 用另一棵子树替换一棵子树并成为其双亲的孩子节点。用 new_subtree 替换 origin_subtree
  auto _rb_transplant = [&](Node<T, I> *origin_subtree, Node<T, I> *new_subtree) {
    if (origin_subtree->parent == null_node_) {
      // origin_subtree 是根节点
      root_ = new_subtree;
    } else if (origin_subtree == origin_subtree->parent->left) {
      // origin_subtree 是左孩子
      origin_subtree->parent->left = new_subtree;
    } else {
      // origin_subtree 是右孩子
      origin_subtree->parent->right = new_subtree;
    }

    // 更新__v的父节点
    new_subtree->parent = origin_subtree->parent;
  };

  // 寻找子树的最小节点
  auto _tree_minimum = [&](Node<T, I> *index_node) -> Node<T, I> * {
    Node<T, I> *node = index_node;
    while (node->left != null_node_) {
      node = node->left;
    }
    return node;
  };

  // 记录删除节点的颜色，后续用来判断是否需要调整红黑树
  COLOR origin_color = erase_node->color;

  // 记录哪个节点代替了 erase_node，后续如果需要调整红黑树，则从该节点开始
  Node<T, I> *start_fixup_node;

  // case 1：删除节点左孩子为空，直接用右孩子代替 erase_node
  if (erase_node->left == null_node_) {
    start_fixup_node = erase_node->right;
    _rb_transplant(erase_node, erase_node->right);
  }
  // case 2：删除节点右孩子为空，直接用左孩子代替 erase_node
  else if (erase_node->right == null_node_) {
    start_fixup_node = erase_node->left;
    _rb_transplant(erase_node, erase_node->left);
  }
  // case 3：删除节点有左右孩子
  else {
    // 找到删除节点右子树的最小节点
    // 因为右子树的所有元素都会比左子树大，所以右子树最小节点将成为替换被删除的节点
    auto replace_node = _tree_minimum(erase_node->right);

    origin_color = replace_node->color;
    start_fixup_node = replace_node->right;

    // case 1：删除节点的右孩子没有左孩子
    // 下面会用 replace_node 替换 erase_node
    if (replace_node->parent == erase_node) {
      start_fixup_node->parent = replace_node;
    }
    // case 2：删除节点的右子树最小节点不是删除节点的右孩子
    // 将 replace_node 摘出来，replace_node 的右孩子替换 replace_node
    // 后面 replace_node 替换 earse_node
    else {
      _rb_transplant(replace_node, replace_node->right);
      replace_node->right = erase_node->right;
      replace_node->right->parent = replace_node;
    }

    // 将 erase_node 摘出来，用 replace_node 代替 erase_node
    // 并调整 erase_node 左孩子和 replace_node 之间的关系
    _rb_transplant(erase_node, replace_node);
    replace_node->left = erase_node->left;
    replace_node->left->parent = replace_node;
    replace_node->color = erase_node->color;
  }

  // 只有删除的节点是黑色的，才需要调整红黑树
  if (origin_color == COLOR::BLACK) {
    _erase_fixup(start_fixup_node);
  }

  delete erase_node;

  return true;
}

// 插入辅助函数
template <typename T, typename I>
void RBTree<T, I>::_insert_fixup(Node<T, I> *insert_node) {
  // 因为插入节点一定先是叶子节点，并且是红色，所以如果父节点一直为红色，则要调整
  auto iter_node = insert_node;  // 用来向上染色，直到没有父子节点都为红色
  while (iter_node->parent->color == COLOR::RED) {
    /*
     * case 1: 父节点位于爷爷节点的左孩子
     *       grandfather
     *      /          \
     *     parent      uncle
     *      |
     *    iter_node
     */
    if (iter_node->parent == iter_node->parent->parent->left) {
      // 获得 uncle 节点
      Node<T, I> *uncle_node = iter_node->parent->parent->right;
      if (uncle_node->color == COLOR::RED) {
        /* case 1: uncle 节点是红的话， 父节点和 uncle 节点变为黑，爷爷节点变红,
         *         上移 iter_node 为爷爷节点
         *            grandfather: BLACK                   grandfather: RED
         *             /          \                          /           \
         *   parent: RED         uncle: RED    -->  parent: BLACK     uncle: BLACK
         *       |                                     |
         * iter_node: RED                          iter_node: RED
         */
        iter_node->parent->color = COLOR::BLACK;
        uncle_node->color = COLOR::BLACK;
        // 爷爷节点变红
        iter_node->parent->parent->color = COLOR::RED;
        // 从爷爷节点继续向上染色
        iter_node = iter_node->parent->parent;
      } else if (iter_node == iter_node->parent->right) {
        /* case 2: uncle 节点为黑并且 iter_node 位于右孩子，上移 iter_node 为父节点，然后左旋
         *            grandfather: BLACK                   grandfather: BLACK
         *             /          \                          /           \
         *   parent: RED         uncle: BLACK    -->    iter_node: RED     uncle: BLACK
         *          \                                   /       \
         *         iter_node: RED             parent_node: RED     iter_node_right_node
         *                                    /           \
         *                        parent_left_node       iter_node_left_node
         */
        // 这里左旋后 iter_node 为 parent，所以会继续再次向上染色
        iter_node = iter_node->parent;
        // 左旋 parent 节点
        _rotate_left(iter_node);
      } else {
        /* case 3: uncle节点为黑并且 iter_node 位于左孩子
         *         父节点变黑，爷爷节点变红，然后右旋 grandfather
         *
         *            grandfather: BLACK                     parent: BLACK
         *             /          \                           /           \
         *   parent: RED         uncle: BLACK --> iter_node:RED       grandfather: RED
         *     /                                  /      \                 /         \
         * iter_node: RED           iter_node_left  iter_node_right   parent_right    uncle: BLACK
         */
        iter_node->parent->color = COLOR::BLACK;
        iter_node->parent->parent->color = COLOR::RED;
        _rotate_right(iter_node->parent->parent);
      }
    }
    /*
     * case 2: 父节点位于爷爷节点的右孩子
     *          grandfather
     *         /           \
     *       uncle         father
     *                       |
     *                   insert_node
     */
    else {
      // 这里跟 case 1 镜像就行
      Node<T, I> *uncle_node = iter_node->parent->parent->left;
      if (uncle_node->color == COLOR::RED) {
        // case 1: 父节点和uncle节点变为黑，上移insert节点
        iter_node->parent->color = COLOR::BLACK;
        uncle_node->color = COLOR::BLACK;
        iter_node->parent->parent->color = COLOR::RED;
        iter_node = iter_node->parent->parent;
      } else if (iter_node == iter_node->parent->left) {
        iter_node = iter_node->parent;
        _rotate_right(iter_node);
      } else {
        iter_node->parent->color = COLOR::BLACK;
        iter_node->parent->parent->color = COLOR::RED;
        _rotate_left(iter_node->parent->parent);
      }
    }
  }

  // 保证根节点为黑色
  root_->color = COLOR::BLACK;
}

// 删除辅助函数
template <typename T, typename I>
void RBTree<T, I>::_erase_fixup(Node<T, I> *fixup_node) {
  // 向上调整
  while (fixup_node != root_ && fixup_node->color == COLOR::BLACK) {
    // fixup_node 位于左孩子
    if (fixup_node == fixup_node->parent->left) {
      auto brother = fixup_node->parent->right;
      /**
       * case 1: 兄弟节点为 red，将父节点变红，兄弟节点染黑，然后左旋父节点
       *          parent: BLACK                            brother: BLACK
       *          /            \                           /          \
       * fixup_node: BLACK     brother: RED -->   parent: RED    brother_right: BLACK
       *                                          /          \
       *                             fixup_node: BLACK   brother_left: BLACK
       */
      if (brother->color == COLOR::RED) {
        brother->color = COLOR::BLACK;
        fixup_node->parent->color = COLOR::RED;
        _rotate_left(fixup_node->parent);
        brother = fixup_node->parent->right;
      }
      /**
       * case 2: 兄弟节点的左右孩子都是为黑色，将兄弟节点染红，fixup_node 更新为父节点
       *         如果 父节点为 RED，终止循环，在最后将 fixup_node 染黑，否则继续
       *              parent                                  parent
       *          /            \                           /          \
       * fixup_node: BLACK   brother: BLACK --> fixup_node: BLACK   brother: RED
       *                      /      \                              /          \
       *   brother_left: BLACK    brother_right: BLACK   brother_left: BLACK    brother_right: BLACK
       */
      // fixup_node 更新为父节点了，继续判断循环
      if (brother->left->color == COLOR::BLACK && brother->right->color == COLOR::BLACK) {
        brother->color = COLOR::RED;
        fixup_node = fixup_node->parent;
      }
      /**
       * case 3: 兄弟节点的左孩子是 RED，右孩子是为 BLACK
       *         将兄弟节点染红，左孩子染黑，右旋 brother 节点，重新更新 brother 节点
       *              parent                                  parent
       *          /            \                           /          \
       * fixup_node: BLACK   brother: BLACK --> fixup_node: BLACK   brother_left: BLACK
       *                      /      \                                   \
       *   brother_left: RED    brother_right: BLACK                    brother: RED
       *                                                                   \
       *                                                            brother_right: BLACK
       */
      else if (brother->right->color == COLOR::BLACK) {
        brother->left->color = COLOR::BLACK;
        brother->color = COLOR::RED;
        _rotate_right(brother);
        brother = fixup_node->parent->right;
      }
      /**
       * case4: 兄弟节点左右孩子都是 RED，将 brother 节点染为父节点颜色
       *        父节点染黑，brother 右孩子染黑，左旋父节点，fixup_node 置为 root_
       *              parent                              brother: parent_color
       *           /           \                             /           \
       * fixup_node: BLACK  brother: BLACK -->           parent         brother_right: BLACK
       *                   /      \                   /           \
       *   brother_left: RED  brother_right: RED fixup_node: BLACK  brother_left: RED
       */
      else {
        brother->color = fixup_node->parent->color;
        fixup_node->parent->color = COLOR::BLACK;
        brother->right->color = COLOR::BLACK;
        _rotate_left(fixup_node->parent);
        fixup_node = root_;
      }
    } else {
      // 跟上面镜像一下就行
      auto brother = fixup_node->parent->left;
      if (brother->color == COLOR::RED) {
        brother->color = COLOR::BLACK;
        fixup_node->parent->color = COLOR::RED;
        _rotate_right(fixup_node->parent);
        brother = fixup_node->parent->left;
      }
      if (brother->left->color == COLOR::BLACK && brother->right->color == COLOR::BLACK) {
        brother->color = COLOR::RED;
        fixup_node = fixup_node->parent;
      } else if (brother->left->color == COLOR::BLACK) {
        brother->right->color = COLOR::BLACK;
        brother->color = COLOR::RED;
        _rotate_left(brother);
        brother = fixup_node->parent->left;
      } else {
        brother->color = fixup_node->parent->color;
        fixup_node->parent->color = COLOR::BLACK;
        brother->left->color = COLOR::BLACK;
        _rotate_right(fixup_node->parent);
        fixup_node = root_;
      }
    }
  }
  // fixup_node 染黑
  fixup_node->color = COLOR::BLACK;
}

// 左旋函数
template <typename T, typename I>
void RBTree<T, I>::_rotate_left(Node<T, I> *rotate_node) {
  // 将 rotate_node 右孩子 replace_node 替换 rotate_node 的位置
  Node<T, I> *replace_node = rotate_node->right;

  // step 1: rotate 右孩子指向 replace_node 的左孩子
  rotate_node->right = replace_node->left;
  if (replace_node->left != null_node_) {
    replace_node->left->parent = rotate_node;
  }

  // step 2: replace_node 父节点指向 rotate_node 的父节点
  replace_node->parent = rotate_node->parent;
  if (rotate_node->parent == null_node_) {
    root_ = replace_node;
  } else if (rotate_node == rotate_node->parent->left) {
    rotate_node->parent->left = replace_node;
  } else {
    rotate_node->parent->right = replace_node;
  }

  // step 3: replace_node 的左孩子指向 rotate_node
  replace_node->left = rotate_node;
  rotate_node->parent = replace_node;
}

// 右旋函数
template <typename T, typename I>
void RBTree<T, I>::_rotate_right(Node<T, I> *rotate_node) {
  // 左旋函数镜像一下就行
  Node<T, I> *replace_node = rotate_node->left;

  // step 1
  rotate_node->left = replace_node->right;
  if (replace_node->right != null_node_) {
    replace_node->right->parent = rotate_node;
  }

  // step 2
  replace_node->parent = rotate_node->parent;
  if (rotate_node->parent == null_node_) {
    root_ = replace_node;
  } else if (rotate_node == rotate_node->parent->left) {
    rotate_node->parent->left = replace_node;
  } else {
    rotate_node->parent->right = replace_node;
  }

  // step 3
  replace_node->right = rotate_node;
  rotate_node->parent = replace_node;
}
}  // namespace tree