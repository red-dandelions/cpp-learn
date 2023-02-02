/**
 * 拓扑排序
 */

#pragma once

#include "graph.h"

#include <queue>

namespace graph {

class TopoLogicalSort {
 public:
  // 可以拓扑排序返回 true
  // graph[u][i] = v; u -> v
  bool operator()(std::vector<int32_t> in, std::vector<std::vector<int32_t>>& graph) {
    std::queue<int32_t> q;
    // 统计无入度的节点
    for (int32_t node = 0; node < in.size(); ++node) {
      if (in[node] == 0) {
        q.push(node);
      }
    }

    while (!q.empty()) {
      int32_t u = q.front();
      q.pop();
      answer.push_back(u);
      for (const auto v : graph[u]) {
        if (--in[v] == 0) {
          q.push(v);
        }
      }
    }

    return answer.size() == graph.size();
  }

  std::vector<int32_t> answer;
};

}  // namespace graph