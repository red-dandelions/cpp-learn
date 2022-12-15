```
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

# gflags
http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

# glog
http_archive(
  name="com_github_google_glog",
  sha256="21bc744fb7f2fa701ee8db339ded7dce4f975d0d55837a97be7d46e8382dea5a",
  strip_prefix="glog-0.5.0",
  urls = ["https://github.com/google/glog/archive/refs/tags/v0.5.0.zip"],
)

# gtest
http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-release-1.11.0",
    urls = ["https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz"],
)

# protobuf
http_archive(
    name = "bazel_skylib",
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz",
    ],
)
http_archive(
    name = "com_google_protobuf",
    strip_prefix = "protobuf-3.19.4",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.19.4.tar.gz"],
)
# rules_cc defines rules for generating C++ code from Protocol Buffers.
http_archive(
    name = "rules_cc",
    sha256 = "35f2fb4ea0b3e61ad64a369de284e4fbbdcdba71836a5555abb5e194cf119509",
    strip_prefix = "rules_cc-624b5d59dfb45672d4239422fa1e3de1822ee110",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_cc/archive/624b5d59dfb45672d4239422fa1e3de1822ee110.tar.gz",
        "https://github.com/bazelbuild/rules_cc/archive/624b5d59dfb45672d4239422fa1e3de1822ee110.tar.gz",
    ],
)
# rules_proto defines abstract rules for building Protocol Buffers.
http_archive(
    name = "rules_proto",
    sha256 = "2490dca4f249b8a9a3ab07bd1ba6eca085aaf8e45a734af92aad0c42d9dc7aaf",
    strip_prefix = "rules_proto-218ffa7dfa5408492dc86c01ee637614f8695c45",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/218ffa7dfa5408492dc86c01ee637614f8695c45.tar.gz",
        "https://github.com/bazelbuild/rules_proto/archive/218ffa7dfa5408492dc86c01ee637614f8695c45.tar.gz",
    ],
)
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
rules_cc_dependencies()
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
# bazel run @hedron_compile_commands//:refresh_all
http_archive(
    name = "hedron_compile_commands",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/12440540f4461cac006a357257d698aed74a2423.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-12440540f4461cac006a357257d698aed74a2423",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()

# grpc
http_archive(
    name = "com_github_grpc_grpc",
    strip_prefix = "grpc-1.49.1",
    urls = ["https://github.com/grpc/grpc/archive/refs/tags/v1.49.1.tar.gz"],
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()


# benchmark
http_archive(
    name = "com_github_google_benchmark",
    strip_prefix = "benchmark-1.7.0",
    urls = ["https://github.com/google/benchmark/archive/refs/tags/v1.7.0.tar.gz"],
)

# json
http_archive(
    name = "nlohmann_json",
    strip_prefix = "json-develop",
    urls = ["https://github.com/nlohmann/json/archive/refs/heads/develop.zip"],
)

# 用来构建外部非 bazel 的项目
http_archive(
    name = "rules_foreign_cc",
    strip_prefix = "rules_foreign_cc-0.8.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.8.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

# prometheus c++ client
http_archive(
    name = "com_github_jupp0r_prometheus_cpp",
    strip_prefix = "prometheus-cpp-master",
    urls = ["https://github.com/jupp0r/prometheus-cpp/archive/master.zip"],
)

load("@com_github_jupp0r_prometheus_cpp//bazel:repositories.bzl", "prometheus_cpp_repositories")

prometheus_cpp_repositories()


# skywalking
http_archive(
  name = "com_github_skyapm_cpp2sky",
  strip_prefix = "cpp2sky-0.5.1",
  urls = ["https://github.com/SkyAPM/cpp2sky/archive/v0.5.1.tar.gz"],
)
load("@com_github_skyapm_cpp2sky//bazel:repositories.bzl", "cpp2sky_dependencies")
cpp2sky_dependencies()
```