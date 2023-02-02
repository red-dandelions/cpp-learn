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
    "network_utils.cc",
    "reactor.cc",
    "event_demultiplex.cc",
  ],
  hdrs = [
    "network_utils.h",
    "reactor.h",
    "event_demultiplex.h",
    "event_handler.h",
  ],
  deps = [
    "@com_github_google_glog//:glog",
  ]
)