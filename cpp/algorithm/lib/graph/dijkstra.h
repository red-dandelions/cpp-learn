/**
 * 求解 非负权图 上单源最短路径的算法。
 *
 * 在稀疏图中，m = O(n)，使用二叉堆实现的 Dijkstra 算法较 Bellman-Ford 算法具有较大的效率优势；
 * 而在稠密图中，m = O(n^2)，这时候使用暴力做法较二叉堆实现更优。
 *
 * 这里同时给出 O(n^2) 的暴力做法实现和 O(m \log m) 的优先队列做法实现。
 */

#pragma once

#include "graph.h"

#include <queue>
#include <vector>

namespace graph {

class Dijkstra {
 public:
  explicit Dijkstra(Graph g)
      : answer(g.graph.size(), int64_t(INT32_MAX)), visit_(g.graph.size()), graph_(g.graph) {}

  void reset() {
    for (int32_t i = 0; i < answer.size(); ++i) {
      answer[i] = INT32_MAX;
      visit_[i] = 0;
    }
  }

  void force(int32_t start_node) {
    answer[start_node] = 0;
    for (int32_t i = 0; i < answer.size(); ++i) {
      int32_t u = -1;
      int64_t m_value = INT32_MAX;
      for (int j = 0; j < answer.size(); ++j) {
        if (!visit_[j] && answer[j] < m_value) {
          u = j;
          m_value = answer[j];
        }
      }
      visit_[u] = 1;
      for (auto p : graph_[u]) {
        int v = p.first;
        int w = p.second;
        if (answer[v] > answer[u] + w) {
          answer[v] = answer[u] + w;
        }
      }
    }
  }

  void dijkstra(int32_t start_node) {
    answer[start_node] = 0;
    // 小根堆
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
    q.push({.weight = 0, .node = start_node});
    while (!q.empty()) {
      int32_t u = q.top().node;
      q.pop();
      if (visit_[u]) {
        continue;
      }

      visit_[u] = 1;
      for (auto p : graph_[u]) {
        int32_t v = p.first;
        int32_t w = p.second;
        if (answer[v] > answer[u] + w) {
          answer[v] = answer[u] + w;
          q.push({.weight = answer[v], .node = v});
        }
      }
    }
  }

  // 从 start_node 出发到各个顶点的距离
  std::vector<int64_t> answer;
  std::vector<int64_t> visit_;
  std::vector<std::vector<std::pair<int32_t, int32_t>>> graph_;

 private:
  struct Node {
    int32_t node;
    int64_t weight;
    bool operator>(const Node& n) const { return weight > n.weight; }
  };
};

}  // namespace graph