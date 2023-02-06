# 使用 ubuntu 作为基本镜像
FROM --platform=linux/amd64 ubuntu:18.04

ENV TZ=Asia/Shanghai LANG=C.UTF-8
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone && \
    # 安装一些依赖
    apt update && \
    apt install clang-format gdb gcc g++ git vim inetutils-ping unzip zip tcpdump wget python tzdata gcc-multilib nodejs npm net-tools -y && \
    # 安装 bazel，版本为 5.3.2
    wget https://github.com/bazelbuild/bazel/releases/download/5.3.2/bazel-5.3.2-installer-linux-x86_64.sh -O bazel-install.sh && \
    bash bazel-install.sh && \
    rm -rf bazel* && \
    # 安装 cmake，版本为 3.24.3
    wget https://github.com/Kitware/CMake/releases/download/v3.24.3/cmake-3.24.3-linux-x86_64.tar.gz -O cmake.tar.gz && \
    tar -xf cmake.tar.gz && mv cmake-3.24.3-linux-x86_64 /opt/cmake && ln -sf /opt/cmake/bin/* /usr/bin/ && \
    rm -rf cmake* && \
    # 安装 gpreftools，性能分析
    apt install autoconf automake libtool make -y && \
    wget https://github.com/gperftools/gperftools/archive/refs/tags/gperftools-2.10.tar.gz -O gperftools.tar.gz && \
    tar -xf gperftools.tar.gz && \
    cd gperftools-gperftools-2.10/ && bash autogen.sh && bash configure && make && make install && cd .. && \
    rm -rf gperftools*  &&\

    # 安装 valgrind，内存泄漏检测
    wget https://sourceware.org/pub/valgrind/valgrind-3.20.0.tar.bz2 && \
    tar -xf valgrind-3.20.0.tar.bz2 && \
    cd valgrind-3.20.0 && ./configure && make && make install && cd .. && \
    rm -rf valgrind*

# 安装python3.7
RUN apt install software-properties-common -y && \
    add-apt-repository ppa:deadsnakes/ppa && \
    apt install python3.7 python3.7-dev -y && rm /usr/bin/python3 && ln -sf /usr/bin/python3.7 /usr/bin/python3 && \
    apt install python3-pip -y \
    # 安装 redis, kafka 和 mysql 的 python 库
    pip3 install redis pymysql kafka-python