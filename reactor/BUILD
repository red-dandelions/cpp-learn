cc_binary(
  name = "reactor",
  srcs = [
    "main.cc",
  ],
  deps = [
    "//reactor:reactor_lib",
    "@com_github_google_glog//:glog",
  ],
)

cc_library(
  name = "reactor_lib",
  srcs = [
    "acceptor.cc",
    "network_utils.cc",
    "reactor.cc",
    "event_demultiplex.cc",
    "handler.cc",
    "processor.cc",
    "thread_pool.cc",
  ],
  hdrs = [
    "acceptor.h",
    "network_utils.h",
    "reactor.h",
    "event_demultiplex.h",
    "handler.h",
    "processor.h",
    "thread_pool.h",
  ],
  deps = [
    "@com_github_google_glog//:glog",
  ],
  linkopts = ["-lpthread"],
)