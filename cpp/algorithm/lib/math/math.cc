#include "math.h"

namespace algorithm {
// 快速幂
int64_t bin_pow(int64_t v, int64_t p) {
  int64_t result = 1;
  while (p > 0) {
    if (p & 1) {
      result *= v;
    }
    v *= v;
    p >>= 1;
  }
  return result;
}
} // namespace algorithm