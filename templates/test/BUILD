cc_test(
  name = "simd_sum_test",
  srcs = ["simd_sum_test.cc"],
  copts = ["-mavx"],
  deps = [
    "//templates:simd_sum",
    "@com_google_googletest//:gtest_main",
  ],
)