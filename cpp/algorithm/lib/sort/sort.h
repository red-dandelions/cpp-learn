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
//
// 适用于数据分布比较均匀的情况
void bucket_sort(std::vector<int32_t> &v, int32_t bucket_size = INT32_MAX) {
  std::vector<std::vector<int32_t>> bucket(bucket_size);
  for (auto i : v) {
    bucket[i % bucket_size].push_back(i);
  }
  int32_t idx = 0;
  for (int i = 0; i < bucket.size(); ++i) {
    insert_sort(bucket[i]);
    for (auto value : bucket[i]) {
      v[idx++] = value;
    }
  }
}

// 基数排序
void radix_sort(std::vector<int32_t> &v, int32_t d) {
  // 取得位数上的值
  auto _fn = [](int32_t value, int32_t i) -> int32_t {
    int32_t val = i == 0 ? 1 : i * 10;
    return (value / val) % 10;
  };

  // 借助计数排序完成对关键字的排序
  auto _stable_sort = [&v, &_fn](int32_t idx) {
    auto tmp = v;
    std::vector<int32_t> count(11, 0);
    for (int32_t i = 0; i < v.size(); ++i) {
      count[_fn(v[i], idx)]++;
    }
    for (int32_t i = 1; i < 11; i++) {
      count[i] += count[i - 1];
    }
    for (int32_t i = v.size() - 1; i >= 0; --i) {
      v[count[_fn(tmp[i], idx)] - 1] = tmp[i];
      --count[_fn(tmp[i], idx)];
    }
  };

  for (int32_t i = 0; i < d; ++i) {
    _stable_sort(i);
  }
}

// 计数排序
void counting_sort(std::vector<int32_t> &v, int64_t max_value) {
  std::vector<int32_t> tmp = v;
  std::vector<int32_t> count(max_value + 1, 0);
  for (int64_t i = 0; i < v.size(); ++i) {
    count[v[i]]++;
  }
  for (int i = 1; i < count.size(); ++i) {
    count[i] += count[i - 1];
  }
  for (int i = v.size() - 1; i >= 0; --i) {
    v[count[tmp[i]] - 1] = tmp[i];
    --count[tmp[i]];
  }
}

}  // namespace sort