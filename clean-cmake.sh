#!/bin/bash

# check for build directories
if [ -d "./build-release" ] 
then 
    rm -rf ./build-release
fi
if [ -d "./build-debug" ] 
then 
    rm -rf ./build-debug
fi
if [ -d "./build" ] 
then 
    rm -rf ./build
fi
if [ -d "./build-gcc-release" ] 
then 
    rm -rf ./build-gcc-release
fi
if [ -d "./build-gcc-debug" ] 
then 
    rm -rf ./build-gcc-debug
fi
if [ -d "./build-clang-release" ] 
then 
    rm -rf ./build-clang-release
fi
if [ -d "./build-clang-debug" ] 
then 
    rm -rf ./build-clang-debug
fi

# Remove log file if it exists
if [ -f test-cmake-linux.log ]; then
    rm test-cmake-linux.log
fi

# Remove cache if it exists
if [ -d ".cache" ] 
then 
    rm -rf .cache
fi
