cc_test(
  name = "binary_index_tree_test",
  srcs = ["binary_index_tree_test.cc"],
  deps = [
    "//cpp/datastructure/tree:tree",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "heap_test",
  srcs = ["heap_test.cc"],
  deps = [
    "//cpp/datastructure/tree:tree",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_binary(
  name = "heap_benchmark",
  srcs = ["heap_benchmark.cc"],
  deps = [
    "//cpp/datastructure/tree:tree",
    "@com_github_google_benchmark//:benchmark",
  ],
)