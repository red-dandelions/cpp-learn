#include "glog/logging.h"

int main() {
  LOG(INFO) << "this is info level";
  LOG(WARNING) << "this is warning level";
  LOG(ERROR) << "this is error level";

  return 0;
}