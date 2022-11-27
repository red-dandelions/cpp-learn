# 使用 ubuntu 作为基本镜像
FROM --platform=linux/amd64 ubuntu:18.04

ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && \
    # 安装一些依赖
    apt update && \
    apt install gdb gcc g++ git vim inetutils-ping unzip zip tcpdump wget python python3 tzdata -y && \
    # 安装 bazel，版本为 5.3.2
    wget https://github.com/bazelbuild/bazel/releases/download/5.3.2/bazel-5.3.2-installer-linux-x86_64.sh -O bazel-install.sh && \
    bash bazel-install.sh && \
    rm -rf bazel* && \
    # 安装 cmake，版本为 3.24.3
    wget https://github.com/Kitware/CMake/releases/download/v3.24.3/cmake-3.24.3-linux-x86_64.tar.gz -O cmake.tar.gz && \
    tar -xf cmake.tar.gz && mv cmake-3.24.3-linux-x86_64 /opt/cmake && ln -sf /opt/cmake/bin/* /usr/bin/ && \
    rm -rf cmake* && \
    # 安装 gpreftools
    apt install autoconf automake libtool make -y && \
    wget https://github.com/gperftools/gperftools/archive/refs/tags/gperftools-2.10.tar.gz -O gperftools.tar.gz && \
    tar -xf gperftools.tar.gz && \
    cd gperftools-gperftools-2.10/ && bash autogen.sh && bash configure && make && make install && cd .. && \
    rm -rf gperftools*