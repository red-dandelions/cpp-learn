load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
  name = "hello_proto",
  srcs = ["hello.proto"],
)

cc_proto_library(
  name = "hello_proto_cc",
  deps = [":hello_proto"],
)

cc_grpc_library(
    name = "hello_grpc",
    srcs = [":hello_proto"],
    grpc_only = True,
    deps = [":hello_proto_cc"],
)

cc_binary(
  name = "main",
  srcs = ["main.cc",
          "hello_impl.h",
  ],
  deps = [
    ":hello_proto_cc",
    ":hello_grpc",
    "@com_github_grpc_grpc//:grpc++",
  ],
)