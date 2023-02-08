#pragma once

#include <algorithm>  // shuffer
#include <chrono>
#include <random>  // default_random_engine
#include <string>
#include <vector>

namespace spades_three {

// 花色: 方片，梅花，红桃，黑桃，大小王
enum class Suit { Diamonds, Clubs, Hearts, Spades, Joker };

// 牌的点数: 4,5,6,7,8,9,10,J,Q,K,A,2,3,king,KING
enum class Number { four, five, six, seven, eight, nine, ten, J, Q, K, A, tow, three, king, KING };

// 一张卡牌
class Card {
 public:
  explicit Card(Number number, Suit suit) : number_(number), suit_(suit) {}

  // 重载比较运算符
  bool operator<(const Card& rhs) const {
    if (this->number_ == rhs.number_) {
      return this->suit_ < rhs.suit_;
    }
    return this->number_ < rhs.number_;
  }

  bool operator>(const Card& rhs) const { return rhs < *this; }

  bool operator==(const Card& rhs) const {
    return this->number_ == rhs.number_ && this->suit_ == rhs.suit_;
  }

 private:
  Number number_;
  Suit suit_;
};

// 两副扑克牌
class Poker {
 public:
  // 构造函数
  Poker() {
    for (int32_t i = 0; i < 4; ++i) {
      for (int32_t j = 0; j < 13; ++j) {
        cards_.emplace_back(Card(static_cast<Number>(j), static_cast<Suit>(i)));
        cards_.emplace_back(Card(static_cast<Number>(j), static_cast<Suit>(i)));
      }
    }
    cards_.emplace_back(Card(Number::king, Suit::Joker));
    cards_.emplace_back(Card(Number::king, Suit::Joker));
    cards_.emplace_back(Card(Number::KING, Suit::Joker));
    cards_.emplace_back(Card(Number::KING, Suit::Joker));

    shuffer();
  }

  std::vector<Card>& Cards() { return cards_; }

 private:
  // 洗牌
  void shuffer() {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::shuffle(cards_.begin(), cards_.end(), std::default_random_engine(seed));
  }

  std::vector<Card> cards_;
};

// 出牌的牌型
//
// 单牌，对子，三张，四带一，三带二，顺子，同花，同花顺
enum class Type {
  Single,    // 单张
  Pair,      // 对子
  Three,     // 三张
  Sequence,  // 顺子
  SameSuit,  // 同花
  ThreeWithPair,
  FourWithOnt,
  Five,
  SuitSequence
};

// 牌型
class CardType {
 public:
  explicit CardType(const std::vector<Card>& cards, Type type) : cards_(cards), type_(type) {}

  Type type() const { return type_; }

 private:
  std::vector<Card> cards_;
  Type type_;
};

}  // namespace spades_three