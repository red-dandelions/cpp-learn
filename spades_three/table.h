#pragma once

#include "player.h"

namespace spades_three {

// 玩家方位
enum class Position { East, South, West, North };

// 一局游戏
class Table {
 public:
  Table(std::vector<Player>& players) {}

  // 开始游戏
  void run() {
    // 发牌

    // 判断哪位玩家首先出牌

    // 顺序出牌

    //
  }

 private:
  std::vector<std::pair<Position, Player>> players_;

  // 分组
};

}  // namespace spades_three