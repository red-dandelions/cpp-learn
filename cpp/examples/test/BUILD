#cc_test(
#  name = "simd_sum_test",
#  srcs = ["simd_sum_test.cc"],
#  copts = ["-mavx"],
#  deps = [
#    "//cpp/examples:simd_sum",
#    "@com_google_googletest//:gtest_main",
#  ],
#)

cc_test(
  name = "singleton_test",
  srcs = [
    "singleton_test.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "thread_pool_test",
  srcs = [
    "thread_pool_test.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "bloom_filter_test",
  srcs = [
    "bloom_filter_test.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "spin_lock_test",
  srcs = [
    "spin_lock_test.cc",
  ],
  deps = [
   "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "blocking_queue_test",
  srcs = [
    "blocking_queue_test.cc",
  ],
  deps = [
   "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "buddy_memory_allocator_test",
  srcs = [
    "buddy_memory_allocator_test.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "object_builder",
  srcs = [
    "object_builder_test.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "timer_test",
  srcs = [
    "timer_test.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_test(
  name = "buddy_memory_allocator_base_object_builder_test",
  srcs = [
    "buddy_memory_allocator_base_object_builder_test.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
    "@com_github_google_glog//:glog",
  ]
)

cc_test(
  name = "shm_msg_queue_test",
  srcs = [
    "shm_msg_queue_test.cc",
  ],
  copts = ["-g"],
  deps = [
    "//cpp/examples:examples",
    "@com_google_googletest//:gtest_main",
    "@com_github_google_glog//:glog",
  ]
)

cc_binary(
  name = "shm_msg_queue_benchmark",
  srcs = [
    "shm_msg_queue_benchmark.cc",
  ],
  deps = [
    "//cpp/examples:examples",
    "@com_github_google_benchmark//:benchmark",
  ],
)