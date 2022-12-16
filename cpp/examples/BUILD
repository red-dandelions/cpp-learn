# mac m1 不支持 intel 指令集
#cc_library(
#  name = "simd_sum",
#  hdrs = ["simd_sum.h"],
#  visibility = ["//visibility:public"],
#  deps = [
#    "@com_github_google_glog//:glog",
#  ],
#)

cc_library(
  name = "examples",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  deps = [
    "//cpp/datastructure/hash:hash",
    "@com_github_google_glog//:glog",
  ],
  visibility = ["//visibility:public"],
)