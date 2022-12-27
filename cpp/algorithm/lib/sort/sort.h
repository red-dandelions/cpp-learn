#pragma once

#include <vector>

namespace sort {

// 冒泡排序
template <typename T>
void bubble_sort(std::vector<T> &v) {
  int32_t n = v.size();
  for (int32_t i = 0; i < n; ++i) {
    for (int32_t j = 1; j < n - i; ++j) {
      if (v[j - 1] > v[j]) {
        std::swap(v[j - 1], v[j]);
      }
    }
  }
}

// 插入排序
template <typename T>
void insert_sort(std::vector<T> &v) {
  int32_t n = v.size();
  for (int32_t i = 1; i < n; ++i) {
    int32_t j = i;
    while (j > 0) {
      if (v[j - 1] > v[j]) {
        std::swap(v[j - 1], v[j]);
      } else {
        break;
      }
      --j;
    }
  }
}

// 选择排序
template <typename T>
void select_sort(std::vector<T> &v) {
  int32_t n = v.size();
  for (int32_t i = 0; i < n; ++i) {
    T value = v[i];
    int32_t index = i;
    for (int32_t j = i + 1; j < n; ++j) {
      if (v[j] < value) {
        value = v[j];
        index = j;
      }
    }
    std::swap(v[i], v[index]);
  }
}

// 希尔排序
template <typename T>
void shell_sort(std::vector<T> &v) {
  int32_t n = v.size();
  for (int32_t d = n >> 1; d >= 1; d >>= 1) {
    for (int32_t i = d; i < n; ++i) {
      int32_t j = i;
      while (j > 0) {
        if (v[j - d] > v[j]) {
          std::swap(v[j - 1], v[j]);
        } else {
          break;
        }
        j -= d;
      }
    }
  }
}

// 归并排序
template <typename T>
void merge_sort(std::vector<T> &v, int32_t start, int32_t end) {
  auto _merge = [](std::vector<T> &v, int32_t left, int32_t mid, int32_t right) {
    int32_t len1 = mid - left;
    int32_t len2 = right - mid;
    T a[len1];
    T b[len2];
    for (int32_t i = 0; i < len1; ++i) {
      a[i] = v[left + i];
    }
    for (int32_t i = 0; i < len2; ++i) {
      b[i] = v[mid + i];
    }
    int32_t index = left;
    int32_t i = 0, j = 0;
    while (index < right) {
      if (i < len1 && j < len2) {
        if (a[i] < b[j]) {
          v[index] = a[i];
          ++i;
        } else {
          v[index] = b[j];
          ++j;
        }
      } else if (i < len1) {
        v[index] = a[i];
        ++i;
      } else {
        v[index] = b[j];
        ++j;
      }
      ++index;
    }
  };
  if (start < end - 1) {
    int32_t mid = start + ((end - start) >> 1);
    merge_sort(v, start, mid);
    merge_sort(v, mid, end);
    _merge(v, start, mid, end);
  }
}

// 堆排序
template <typename T>
void heap_sort(std::vector<T> &arr) {
  auto _heapify = [](std::vector<T> &arr, int32_t parent, int32_t end) {
    int32_t child = 2 * parent + 1;
    while (child < end) {
      if (child + 1 < end && arr[child] < arr[child + 1]) {
        ++child;
      }
      if (arr[parent] < arr[child]) {
        std::swap(arr[parent], arr[child]);
      }
      parent = child;
      child = 2 * parent + 1;
    }
  };

  int32_t n = arr.size();
  for (int32_t i = n / 2 - 1; i >= 0; --i) {
    _heapify(arr, i, n);
  }
  for (int32_t i = n - 1; i > 0; --i) {
    std::swap(arr[0], arr[i]);
    _heapify(arr, 0, i);
  }
}

// 快速排序
template <typename T>
void quick_sort(std::vector<T> &arr, int32_t start, int32_t end) {
  auto _partition = [](std::vector<T> &arr, int32_t left, int32_t right) {
    int32_t index = left + rand() % (right - left + 1);
    std::swap(arr[left], arr[index]);

    int32_t val = arr[left];
    while (left < right) {
      while (left < right && val <= arr[right]) {
        --right;
      }
      std::swap(arr[left], arr[right]);

      while (left < right && arr[left] <= val) {
        ++left;
      }
      std::swap(arr[left], arr[right]);
    }
    arr[left] = val;
    return left;
  };

  if (start < end) {
    int32_t mid = _partition(arr, start, end);
    quick_sort(arr, start, mid - 1);
    quick_sort(arr, mid + 1, end);
  }
}

// 桶排序
namespace bucket {
const int32_t N = 100010;
int32_t n, w, a[N];
std::vector<int32_t> bucket[N];

void bucket_sort() {
  int32_t bucket_size = w / n + 1;
  for (int32_t i = 0; i < n; ++i) {
    bucket[i].clear();
  }
  for (int32_t i = 1; i <= n; ++i) {
    bucket[a[i] / bucket_size].push_back(a[i]);
  }
  int32_t p = 0;
  for (int32_t i = 0; i < n; ++i) {
    insert_sort(bucket[i]);
    for (int32_t j = 0; j < bucket[i].size(); ++j) {
      a[++p] = bucket[i][j];
    }
  }
}
}  // namespace bucket

// 基数排序
namespace radix {
const int32_t N = 100010;
const int32_t W = 100010;
const int32_t K = 100;
int32_t n, w[K], k, cnt[W];
struct Element {
  int32_t key[K];
  bool operator<(const Element &y) const {
    // 两个元素的比较流程
    for (int32_t i = 1; i <= k; ++i) {
      if (key[i] == y.key[i]) continue;
      return key[i] < y.key[i];
    }
    return false;
  }
} a[N], b[N];

void counting_sort(int32_t p) {
  memset(cnt, 0, sizeof(cnt));
  for (int32_t i = 1; i <= n; ++i) ++cnt[a[i].key[p]];
  for (int32_t i = 1; i <= w[p]; ++i) cnt[i] += cnt[i - 1];
  // 为保证排序的稳定性，此处循环i应从n到1
  // 即当两元素关键字的值相同时，原先排在后面的元素在排序后仍应排在后面
  for (int32_t i = n; i >= 1; --i) b[cnt[a[i].key[p]]--] = a[i];
  memcpy(a, b, sizeof(a));
}

void radix_sort() {
  for (int32_t i = k; i >= 1; --i) {
    // 借助计数排序完成对关键字的排序
    counting_sort(i);
  }
}
}  // namespace radix

// 计数排序
namespace count {
const int32_t N = 100010;
const int32_t W = 100010;

int32_t n, w, a[N], cnt[W], b[N];

void counting_sort() {
  memset(cnt, 0, sizeof(cnt));
  for (int32_t i = 1; i <= n; ++i) ++cnt[a[i]];
  for (int32_t i = 1; i <= w; ++i) cnt[i] += cnt[i - 1];
  for (int32_t i = n; i >= 1; --i) b[cnt[a[i]]--] = a[i];
}
}  // namespace count

}  // namespace sort