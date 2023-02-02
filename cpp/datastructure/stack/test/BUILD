cc_test(
  name = "monotonic_stack_test",
  srcs = ["monotonic_stack_test.cc"],
  copts = ["-pthread"],
  deps = [
    "@com_google_googletest//:gtest_main",
  ],
)

cc_binary(
  name = "stack_benchmark",
  srcs = ["stack_benchmark.cc"],
  deps = [
    "//cpp/datastructure/stack:stack",
    "@com_github_google_benchmark//:benchmark",
  ],
)