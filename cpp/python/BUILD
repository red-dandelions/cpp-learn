load("@pybind11_utils//:build_defs.bzl", "pybind11_extension_with_soabi")

pybind11_extension_with_soabi(
  name = "example",
  srcs = [
    "pybind11_test.cc",
  ],
  linkstatic = True,
)

py_library(
  name = "pybind",
  srcs = ["__init__.py"],
  data = [
    ":example",
  ],
  visibility = ["//visibility:public"],
)

py_binary(
  name = "pybind11_test",
  srcs = [
    "pybind11_test.py",
  ],
  deps = [
    ":pybind",
  ],
)