#pragma once

#include <vector>

namespace tree {
template <typename T>
class SegmentTree {
 public:
  void init(std::vector<T>& v, bool init_value = true) {
    tree_.resize(v.size() * 4, 0);
    lazy_.resize(v.size() * 4, 0);
    if (!init_value) {
      // 不需要初始化值，则返回
      return;
    }
    std::function<void (int32_t, int32_t, int32_t)> _build;
    _build = [&](int32_t root, int32_t cl, int32_t cr) -> void {
      if (cl == cr) {
        tree_[root] = v[cl];
        return;
      }
      int32_t cm = cl + (cr - cl) / 2;
      _build(root * 2 + 1, cl, cm);
      _build(root * 2 + 2, cm + 1, cr);

      tree_[root] = tree_[root * 2 + 1] + tree_[root * 2 + 2];
    };

    _build(0, 0, v.size() - 1);
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

    return _query(0, tree_.size() / 4 - 1, 0);
  }

  void range_add(int32_t l, int32_t r, T value) {
    std::function<void(int32_t, int32_t, int32_t)> _update;
    _update = [&](int32_t cl, int32_t cr, int32_t root) {
      if (l <= cl && cr <= r) {
        lazy_[root] += value;
        tree_[root] += (cr - cl + 1) * value;
        return;
      }

      _maintain(cl, cr, root);

      int cm = cl + (cr - cl) / 2;
      if (l <= cm) _update(cl, cm, root * 2 + 1);
      if (cm < r) _update(cm + 1, cr, root * 2 + 2);
      tree_[root] = tree_[root * 2 + 1] + tree_[root * 2 + 2];
    };

    _update(0, tree_.size() / 4 - 1, 0);
  }

  void range_set(int32_t l, int32_t r, T value) {
    // TODO: 区间求和的 lazy_ 
    auto _set_maintain = [&](int32_t cl, int32_t cr, int32_t root) {
      int cm = cl + (cr - cl) / 2;
      if (cl != cr && lazy_[root]) {
        lazy_[root * 2 + 1] = lazy_[root];
        lazy_[root * 2 + 2] = lazy_[root];
        tree_[root * 2 + 1] = (cm - cl + 1) * lazy_[root];
        tree_[root * 2 + 2] = (cr - cm) * lazy_[root];
        lazy_[root] = 0;
      }
    };

    std::function<void(int32_t, int32_t, int32_t)> _update;
    _update = [&](int32_t cl, int32_t cr, int32_t root) {
      if (l <= cl && cr <= r) {
        lazy_[root] = value;
        tree_[root] = (cr - cl + 1) * value;
        return;
      }

      _set_maintain(cl, cr, root);

      int32_t cm = cl + (cr - cl) / 2;
      if (cl <= cm) _update(cl, cm, root * 2 + 1);
      if (cm < cr) _update(cm + 1, cr, root * 2 + 2);
      tree_[root] = tree_[root * 2 + 1] + tree_[root * 2 + 2];
    };
    _update(0, tree_.size() / 4 - 1, 0);
  }

 private:
  void _maintain(int32_t cl, int32_t cr, int32_t root) {
    int cm = cl + (cr - cl) / 2;
    if (cl != cr && lazy_[root]) {
      // 更新左右孩子的 lazy 标记
      // 更新左右孩子的值
      // 将父节点的 lazy 标记清空
      lazy_[root * 2 + 1] += lazy_[root];
      lazy_[root * 2 + 2] += lazy_[root];
      tree_[root * 2 + 1] += lazy_[root] * (cm - cl + 1);
      tree_[root * 2 + 2] += lazy_[root] * (cr - cm);
      lazy_[root] = 0;
    }
  }
  std::vector<T> tree_;
  std::vector<T> lazy_;  // 懒惰标记
};

}  // namespace tree