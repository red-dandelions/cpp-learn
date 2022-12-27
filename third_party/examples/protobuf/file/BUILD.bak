proto_library(
  name = "hello_proto",
  srcs = [
    "hello.proto",
  ],
)

cc_proto_library(
  name = "hello_proto_cc",
  visibility = ["//visibility:public"],
  deps = [":hello_proto"],
)