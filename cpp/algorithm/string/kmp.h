#pragma once

#include <string>
#include <vector>

namespace algorithm {

std::vector<int32_t> prefix_function(std::string s) {
  int32_t n = s.length();
  std::vector<int32_t> pi(n);
  for (int32_t i = 1; i < n; i++) {
    int32_t j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}

void compute_automaton(std::string s, std::vector<std::vector<int>>& aut) {
  s += '#';
  int n = s.size();
  std::vector<int> pi = prefix_function(s);
  aut.assign(n, std::vector<int>(26));
  for (int i = 0; i < n; i++) {
    for (int c = 0; c < 26; c++) {
      if (i > 0 && 'a' + c != s[i])
        aut[i][c] = aut[pi[i - 1]][c];
      else
        aut[i][c] = i + ('a' + c == s[i]);
    }
  }
}

void kmp()

}  // namespace algorithm