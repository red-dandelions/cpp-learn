#!/usr/bin/bash
find ./ -iname '*.cc' -o -iname '*.cpp' -o -iname '*.c' -o '*.h' | xargs clang-format -style=file -i