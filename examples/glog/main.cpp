#include "glog/logging.h"

int main(){
    // LOG(DEBUG) << "this is INFO level!";
    LOG(INFO) << "thisd is INFO level!";
    LOG(WARNING) << "this is WARNING level!";
    LOG(ERROR) << "this is WARNING level!";
    // LOG(FATAL) << "this is FATAL level!";

    return 0;
}