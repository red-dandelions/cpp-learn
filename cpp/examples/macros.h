#ifndef __TEMPLATES_MACROS_H__
#define __TEMPLATES_MACROS_H__
#include <bitset>
#include <iostream>

#ifndef BIN
#define BIN(value) (std::cout << std::bitset<32>((value)))  // 输出二进制
#endif
#ifndef OCT
#define OCT(value) (std::cout << oct << (value))  // 输出八进制
#endif
#ifndef HEX
#define HEX(value) (std::cout << hex << (value))  // 输出十六进制
#endif

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#endif  // __TEMPLATES_MACROS_H__