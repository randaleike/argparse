#!/bin/bash
echo "Build gcc debug"
sh ./make-clean.sh Debug gcc build-gcc-debug

echo "Build gcc release"
sh ./make-clean.sh Release gcc build-gcc-release

echo "Build clang debug"
sh ./make-clean.sh Debug clang build-clang-debug

echo "Build clang release"
sh ./make-clean.sh Release clang build-clang-release
