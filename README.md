# cpp-learn
用来记录学习 c++

# 构建新镜像
```bash
docker-compose build develop-image
```

# 拉取最新镜像
```bash
docker-compose pull develop
```

# 启动环境
```bash
docker-compose up -d develop
``` 

# 进入开发环境，以下命令其中一个都可以
```bash
docker-compose run --rm develop bash

docker-compose exec develop bash
```

# 生成 compile_commands.json 文件
```bash
bazel run @hedron_compile_commands//:refresh_all
```

# 启动多组容器
```bash
docker-compose up
```

# 格式化代码
```bash
# 格式化所有源文件
find ./ -iname '*.cc' -o -iname '*.cpp' -o -iname '*.c' -o -iname '*.h' | xargs clang-format -style=file -i

# 格式化单个文件
clang-format -style=file -i {filename}
```