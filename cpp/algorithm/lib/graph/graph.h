#pragma once

#include <vector>

namespace graph {

// 使用邻接表
//
// graph[u][i].first == v; 存在边 (u,v)
// graph[u][i].second == w; (u,v) 权重
struct Graph {
  std::vector<std::vector<std::pair<int32_t, int32_t>>> graph;

  Graph() {}

  //[u, v, w]
  Graph(std::vector<std::vector<int>>& vec, int32_t n) {
    graph.resize(n);
    int32_t w;
    for (const auto& v : vec) {
      w = v.size() > 2 ? v[2] : 0;
      graph[v[0] - 1].push_back(std::make_pair(v[1] - 1, w));
    }
  }
};

}  // namespace graph