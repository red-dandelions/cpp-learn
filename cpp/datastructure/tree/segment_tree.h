#pragma once

#include <vector>

namespace tree {
template <typename T>
class SegmentTree {
 public:
  void build(int32_t root, int32_t cl, int32_t cr) {
    if (cl == cr) {
      tree_[root] = data_[cl];
      return;
    }
    int32_t cm = cl + (cr - cl) / 2;
    build(root * 2 + 1, cl, cm);
    build(root * 2 + 2, cm + 1, cl);

    tree_[root] = tree_[root * 2 + 1] + tree_[root * 2 + 2];
  }

  // [query_l, query_r] 为查询区间
  // [left, right] 为当前节点包含的区间
  // root 为当前节点的编号
  T range_sum(int32_t l, int32_t r) {
    std::function<T(int32_t, int32_t, int32_t)> _query;
    _query = [&](int32_t cl, int32_t cr, int32_t root) -> T {
      // 当前区间为询问区间的子集时直接返回当前区间的和
      if (l <= cl && cr <= r) {
        return tree_[root];
      }

      // 维护 lazy
      _maintain(cl, cr, root);

      int32_t cm = cl + (cr - cl) / 2;
      T sum = 0;
      // 如果左儿子代表的区间 [left, mid] 与询问区间有交集, 则递归查询左儿子
      if (l <= cm) {
        sum += _query(cl, cm, root * 2 + 1);
      }
      // 如果右儿子代表的区间 [m + 1, t] 与询问区间有交集, 则递归查询右儿子
      if (cm < r) {
        sum += _query(cm + 1, cr, root * 2 + 2);
      }
      return sum;
    };

    return _query(0, data_.size() - 1, 0);
  }

  void range_add(int32_t l, int32_t r, T value) {
    std::function<void(int32_t, int32_t)> _update;
    _update = [&](int32_t cl, int32_t cr, int32_t root) {

    };

    _update(0, data_.size() - 1, 0);
  }

  void range_set(int32_t l, int32_t r, T value) {
    std::function<void(int32_t, int32_t)> _update;
    _update = [&](int32_t cl, int32_t cr, int32_t root) {

    };
    _update(0, data_.size(), 0);
  }

 private:
  void _maintain(int32_t cl, int32_t cr, int32_t root) {
    int cm = cl + (cr - cl) / 2;
    if (cl != cr && lazy_[root]) {
      lazy_[root * 2 + 1] += lazy_[root];
      lazy_[root * 2 + 2] += lazy_[root];
      tree_[root * 2 + 1] += lazy_[root] * (cm - cl + 1);
      tree_[root * 2 + 2] += lazy_[root] * (cr - cm);
      lazy_[root] = 0;
    }
  }
  std::vector<T> data_;  // 存放原始数据
  std::vector<T> tree_;
  std::vector<T> lazy_;  // 懒惰标记
};

}  // namespace tree