#pragma once

#include "poker.h"

namespace spades_three {

// 玩家身份
enum class Identity {
  Civilian,  // 没有黑桃三
  Spades,    // 持有一张黑桃三
  Landlord   // 只有两张黑桃三，地主
};

// 玩家
class Player {
 public:
  bool empty() { return cards_.empty(); }

 private:
  int64_t id_;     // id
  int32_t score_;  // 积分

  // 本局游戏中的身份
  Identity identity_;
  // 玩家持有卡牌
  std::vector<Card> cards_;
};

}  // namespace spades_three