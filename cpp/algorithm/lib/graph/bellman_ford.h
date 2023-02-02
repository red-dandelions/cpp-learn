/*
 * 可以求出有负权的图的最短路，并可以对最短路不存在的情况进行判断
 *
 * 如果需要判断整个图上是否存在负环，最严谨的做法是建立一个超级源点，
 * 向图上每个节点连一条权值为 0 的边，然后以超级源点为起点执行 Bellman-Ford 算法。
 */
#pragma once

#include "graph.h"

#include <queue>

namespace graph {

class BellmanFord {
 public:
  explicit BellmanFord(Graph g) : answer(g.graph.size(), int64_t(INT32_MAX)), graph_(g.graph) {}

  // 返回 false 说明没有负环
  bool operator()(int32_t start_node) {
    answer[start_node] = 0;
    bool flag;  // 判断一轮循环过程中是否发生松弛操作
    for (int32_t i = 0; i < answer.size(); i++) {
      flag = false;
      for (int32_t u = 0; u < answer.size(); u++) {
        if (answer[u] == INT32_MAX) continue;

        // 无穷大与常数加减仍然为无穷大
        // 因此最短路长度为 inf 的点引出的边不可能发生松弛操作
        for (const auto& p : graph_[u]) {
          int32_t v = p.first;
          int32_t w = p.second;
          if (answer[v] > answer[u] + w) {
            answer[v] = answer[u] + w;
            flag = true;
          }
        }
      }
      // 没有可以松弛的边时就停止算法
      if (!flag) break;
    }
    // 第 n 轮循环仍然可以松弛时说明 s 点可以抵达一个负环
    return flag;
  }

  // 从 start_node 出发到各个顶点的距离
  std::vector<int64_t> answer;
  std::vector<std::vector<std::pair<int32_t, int32_t>>> graph_;
};

class SPFA {
 public:
  explicit SPFA(Graph g) : answer(g.graph.size(), int64_t(INT32_MAX)), graph_(g.graph) {}

  bool operator()(int32_t start_node) {
    std::vector<int32_t> visit(answer.size(), 0);
    std::vector<int32_t> count(answer.size(), 0);
    std::queue<int32_t> q;

    answer[start_node] = 0;
    visit[start_node] = 1;
    q.push(start_node);

    while (!q.empty()) {
      int32_t u = q.front();
      q.pop();

      visit[u] = 0;
      for (const auto& p : graph_[u]) {
        int32_t v = p.first;
        int32_t w = p.second;

        if (answer[v] > answer[u] + w) {
          answer[v] = answer[u] + w;

          // 记录最短路经过的边数
          // 在不经过负环的情况下，最短路至多经过 n - 1 条边
          // 因此如果经过了多于 n 条边，一定说明经过了负环
          count[v] = count[u] + 1;
          if (count[v] >= answer.size()) {
            return false;
          }

          if (!visit[v]) {
            q.push(v);
            visit[v] = 1;
          }
        }
      }
    }
    return true;
  }

  // 从 start_node 出发到各个顶点的距离
  std::vector<int64_t> answer;
  std::vector<std::vector<std::pair<int32_t, int32_t>>> graph_;
};

}  // namespace graph