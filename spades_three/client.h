#pragma once

#include "glog/logging.h"
#include "network.h"
#include "spades_three/proto/poker.pb.h"

#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>

namespace spades_three {

class Client {
 public:
  Client(const int32_t id)
      : start_(false), stop_(false), connect_(false), hand_up_(false), id_(id) {
    ip_ = "127.0.0.1";
    port_ = "9999";
  }
  ~Client() {
    if (main_thread_) {
      main_thread_->join();
    }
    disconnect();
  }

  // 开始游戏
  void Game() {
    main_thread_.reset(new std::thread(&Client::start, this));

    // 没开局可以取消排队
    while (!start_) {
      // 如果需要取消对局
      // TODO
      bool cancel = false;
      if (cancel) {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
      }
      if (stop_) {
        return;
      }
    }

    LOG(INFO) << "begin game";
  }

 private:
  void start() {
    // 连接服务器
    connect();

    // 发送 id，保证长度为 10
    send_id();

    // server 端凑齐四个玩家之后，返回一个 topic，保证长度为 16
    wait_for_others();

    if (stop_) {
      LOG(INFO) << "stop game by force";
      disconnect();
      return;
    }

    {
      std::lock_guard<std::mutex> lock(mutex_);
      start_ = true;
    }

    // 出牌
    run();

    // 结束对局
  }

  void connect() {
    LOG(INFO) << "connect to server";
    server_ = Connect(ip_, port_);
    connect_ = true;
  }

  void send_id() {
    LOG(INFO) << "send id to server";
    std::string id = std::to_string(id_);
    if (id.size() != 10) {
      id = std::string('0', 10 - id.size()) + id;
    }
    CHECK(id.size() == 10);
    CHECK(send(server_, id.c_str(), 10, 0) == 10) << "send error: " << std::strerror(errno);
  }

  void wait_for_others() {
    LOG(INFO) << "wait for others";
    int32_t bytes = 0;
    // 保证长度为 16
    std::string topic;
    char buf[17];
    while (bytes < 16 && !stop_) {
      std::memset(buf, 0, 17);
      int32_t byte = read(server_, buf, 17);
      if (byte <= 0) {
        // 没有数据可读，再试一次（没准备好）
        if (errno == EAGAIN) {
          continue;
        }
        LOG(ERROR) << "error: " << std::strerror(errno);
        stop_ = true;
      }
      topic += std::string(buf);
      bytes += byte;
    }

    if (stop_) {
      return;
    }

    CHECK(topic.size() == 16);

    topic_ = topic;
  }

  void disconnect() { 
    close(server_);
    connect_ = false;
  }

  void run() {
    // auto card_type_consumer = createConsumer(topic_card_type_);
    // auto card_type_productor = createProductor(topic_card_type_);
    Player player;
    for (int32_t i = 0; i < 4; ++i) {
      std::string s;
      player.ParseFromString(s);
      if (player.id() == std::to_string(id_)) {
        break;
      }
    }
    CHECK(player.id() == std::to_string(id_));

    if (player.first()) {
      Position position = player.position();
      std::string s;
      // 出牌
    }

    while (true) {
      bool finish = true;
      if (finish) {
        break;
      }

      std::string s;
      Cards cards;
      cards.ParseFromString(s);
      LOG(INFO) << s;
    }

    // 出完牌
    LOG(INFO) << "game over";
  }

  bool start_;
  bool stop_;
  bool connect_;
  bool hand_up_;
  const int32_t id_;
  std::mutex mutex_;
  std::unique_ptr<std::thread> main_thread_;
  int32_t server_;
  Player player_;
  std::string ip_;
  std::string port_;
  std::string topic_;
};

}  // namespace spades_three