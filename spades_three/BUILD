cc_binary(
  name = "main",
  srcs = [
    "main.cc",
    "client.h",
    "network.h",
    ],
  deps = [
    "@com_github_google_glog//:glog",
    "//spades_three/proto:poker_proto_cc",
    "@com_github_edenhill_librdkafka//:librdkafka",
  ]
)