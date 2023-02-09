#pragma once

#include <string>

#include "librdkafka/rdkafkacpp.h"

namespace spades_three {

void function(std::string topic_str) {
  RdKafka::Conf* topic_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  std::string errstr;
  RdKfka::Producer *producer;

  producer = RdKafka::Producer::create(topic_conf, errstr);
  RdKafka::Topic *topic =
      RdKafka::Topic::create(producer, topic_str, topic_conf, errstr);

  std::string payload;
  RdKafka::ErrorCode error_code = producer->produce(topic, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY, 
										payload, payload.size(), key, NULL);
}

} // namespace spades_three