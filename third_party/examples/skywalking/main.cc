#include "cpp2sky/config.pb.h"

int main() {
  using namespace cpp2sky;

  static const std::string service_name = "service_name";
  static const std::string instance_name = "instance_name";
  static const std::string oap_addr = "oap:12800";
  static const std::string token = "token";

  TracerConfig config;

  config.set_instance_name(instance_name);
  config.set_service_name(service_name);
  config.set_address(oap_addr);
  config.set_token(token);
}