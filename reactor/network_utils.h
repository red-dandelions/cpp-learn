#pragma once

#include <cinttypes>

namespace reactor {

// 设置文件描述符为非阻塞
void SetNonblock(int32_t fd);

}  // namespace reactor