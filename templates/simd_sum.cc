#include "simd_sum.h"

#include <immintrin.h>
#include <omp.h>
#include <xmmintrin.h>

#include <cstdint>

namespace templates {
namespace functor {
namespace {
// 使用 float 类型作为一个例子
template <size_t N>
inline void ksum_grad_add_kernel(float* __restrict__ dst, const float* __restrict__ src);

template <>
inline void ksum_grad_add_kernel<0>(float* __restrict__ dst, const float* __restrict__ src) {}
template <>
inline void ksum_grad_add_kernel<1>(float* __restrict__ dst, const float* __restrict__ src) {
  *dst += (*src);
}
template <>
inline void ksum_grad_add_kernel<4>(float* __restrict__ dst, const float* __restrict__ src) {
  __m128 a = _mm_loadu_ps(dst);
  __m128 b = _mm_loadu_ps(src);

  a = _mm_add_ps(a, b);
  _mm_storeu_ps(dst, a);
}
template <>
inline void ksum_grad_add_kernel<8>(float* __restrict__ dst, const float* __restrict__ src) {
  __m256 a = _mm256_loadu_ps(dst);
  __m256 b = _mm256_loadu_ps(src);

  a = _mm256_add_ps(a, b);
  _mm256_storeu_ps(dst, a);
}

template <int32_t N>
inline void sum_add_kernel(float* __restrict__ dst, const float* __restrict__ src) {
  if constexpr (N >= 8) {
    sum_add_kernel<8>(dst, src);
    sum_add_kernel<N - 8>(dst + 8, src + 8);
  } else if constexpr (N >= 4) {
    sum_add_kernel<4>(dst, src);
    sum_add_kernel<N - 4>(dst + 4, src + 4);
  } else {
    sum_add_kernel<1>(dst, src);
    sum_add_kernel<N - 1>(dst + 1, src + 1);
  }
}

// 使用 SIMD 指令集优化
template <int32_t kBlockSize>
inline void sum_kernel(float* __restrict__ dst,const float* __restrict__ src) {
  sum_add_kernel<kBlockSize>(dst, src);
}  // sum_kernel

inline void sum_kernel_n(const int32_t block_size, float* __restrict__ dst, const float* __restrict__ src) {
#define CASE_BLOCK_SIZE(SIZE)         \
  case SIZE: {                        \
    sum_kernel<SIZE>(dst, src);       \
  } break;

#define CASE_BLOCK_SIZE_GROUP_4(SIZE) \
  CASE_BLOCK_SIZE(SIZE)               \
  CASE_BLOCK_SIZE(SIZE + 1)           \
  CASE_BLOCK_SIZE(SIZE + 2)           \
  CASE_BLOCK_SIZE(SIZE + 3)

  // 用 switch 匹配不同 block size 的 kernel
  switch (block_size) {
    CASE_BLOCK_SIZE_GROUP_4(1)
    CASE_BLOCK_SIZE_GROUP_4(5)
    CASE_BLOCK_SIZE_GROUP_4(9)
    CASE_BLOCK_SIZE_GROUP_4(13)
  }

#undef CASE_BLOCK_SIZE_GROUP_4
#undef CASE_BLOCK_SIZE
}  // sum_kernel_n
} // namespace

// 普通实现
template <typename T>
void SumImpl<T>::operator()(T* __restrict__ dst, const T* __restrict__ src, int32_t size) {
  for (size_t i = 0; i < size; ++i) {
    *dst += *src;
  }
}

// 一个例子
// 针对 float 类型特化
template <>
void SumImpl<float>::operator()(float* __restrict__ dst, const float* __restrict__ src, int32_t size) {
  // 以 kCacheSize 为单位计算
  constexpr int32_t block_size = 16;
  for (int32_t i = 0; i < size; i += block_size) {
    sum_kernel_n(block_size, dst, src);
  }
}

} // namespace functor
} // namespace templates