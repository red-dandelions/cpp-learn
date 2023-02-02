/**
 * 有向图中求强连通分量的 Tarjan 算法。
 */

#pragma once

#include <vector>

namespace graph {
/**
 * 在 Tarjan 算法中为每个结点 u 维护了以下几个变量：
 *
 * dfn_u：深度优先搜索遍历时结点 u 被搜索的次序。
 * low_u：在 u 的子树中能够回溯到的最早的已经在栈中的结点。
 * 设以 u 为根的子树为 Subtree_u。low_u 定义为以下结点的 dfn 的最小值：Subtree_u 中的结点；
 * 从 Subtree_u 通过一条不在搜索树上的边能到达的结点。
 *
 * 一个结点的子树内结点的 dfn 都大于该结点的 dfn。
 *
 * 从根开始的一条路径上的 dfn 严格递增，low 严格非降。
 */
class Tarjan {
 public:
  void operator()(int32_t root) {}

 private:
  std::vector<int32_t> dfn_;
  std::vector<int32_t> low_;
  std::vector<int32_t> in_stack_;
  std::vector<int32_t> s_;
};

{
  int dfn[N], low[N], dfncnt, s[N], in_stack[N], tp;
  int scc[N], sc;  // 结点 i 所在 SCC 的编号
  int sz[N];       // 强连通 i 的大小

  void tarjan(int u) {
    low[u] = dfn[u] = ++dfncnt, s[++tp] = u, in_stack[u] = 1;
    for (int i = h[u]; i; i = e[i].nex) {
      const int &v = e[i].t;
      if (!dfn[v]) {
        tarjan(v);
        low[u] = min(low[u], low[v]);
      } else if (in_stack[v]) {
        low[u] = min(low[u], dfn[v]);
      }
    }
    if (dfn[u] == low[u]) {
      ++sc;
      while (s[tp] != u) {
        scc[s[tp]] = sc;
        sz[sc]++;
        in_stack[s[tp]] = 0;
        --tp;
      }
      scc[s[tp]] = sc;
      sz[sc]++;
      in_stack[s[tp]] = 0;
      --tp;
    }
  }
}

}  // namespace graph