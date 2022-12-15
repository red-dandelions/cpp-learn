#!/bin/bash
dirname=$1
find ./$1 -iname '*.cc' -o -iname '*.cpp' -o -iname '*.c' -o -iname '*.h' | xargs clang-format -style=file -i