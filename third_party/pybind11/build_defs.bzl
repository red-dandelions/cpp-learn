"""一些工具函数"""

load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

def pybind11_extension_with_soabi(
        name,
        **kwargs):
    soabi_name = name + "." + "%{SOABI}"
    pybind_extension(name = soabi_name, **kwargs)

    native.alias(
        name = name,
        actual = soabi_name + ".so",
        visibility = kwargs["visibility"] if "visibility" in kwargs else None,
    )
