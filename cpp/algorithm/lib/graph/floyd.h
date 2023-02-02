/*
 * 适用于任何图，不管有向无向，边权正负，但是最短路必须存在。
 */
#pragma once

#include "graph.h"

namespace graph {

class Floyd {
 public:
  explicit Floyd(Graph g)
      : answer(g.graph.size(), std::vector(g.graph.size(), int64_t(INT32_MAX))) {
    for (int32_t u = 0; u < g.graph.size(); ++u) {
      for (int32_t i = 0; i < g.graph[u].size(); ++i) {
        int32_t v = g.graph[u][i].first;
        int32_t w = g.graph[u][i].second;
        answer[u][v] = w;
      }
    }
    for (int32_t u = 0; u < answer.size(); ++u) {
      answer[u][u] = 0;
    }
  }

  void operator()() {
    int32_t n = answer.size();
    for (int32_t k = 0; k < n; k++) {
      for (int32_t u = 0; u < n; u++) {
        for (int32_t v = 0; v < n; v++) {
          answer[u][v] = std::min<int64_t>(answer[u][v], answer[u][k] + answer[k][v]);
        }
      }
    }
  }

  // 存储最短距离的数组
  std::vector<std::vector<int64_t>> answer;
  const int32_t INF = INT32_MAX;
};

}  // namespace graph