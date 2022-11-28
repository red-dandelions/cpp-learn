// intel 指令集文档：https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
// 使用 C++ 模板 + SIMD 优化运算
#ifndef __TEMPLATES_SIMD_SUM_H__
#define __TEMPLATES_SIMD_SUM_H__

#include "glog/logging.h"
#include <vector>

namespace templates {

namespace functor {
template <typename T>
struct SumImpl {
  void operator()(T* dst, const T* src, int32_t size);
};
} // namespace functor

template <typename T>
void Sum(std::vector<T>& dst, const std::vector<T>& src) {
  CHECK(dst.size() == src.size()) << "dst.size() != src.size()!";
  // 使用 SIMD 加速
  functor::SumImpl<T>()(dst.data(), src.data(), src.size());
}

} // namespace templates

#endif