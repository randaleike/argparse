#!/bin/bash

#check the input
if [ "$1" != "Debug" && "$1" != "Release" ]
then
    echo "usage: make-clean.sh <Debug|Release> <gcc|clang>"
    exit 1
fi

case "$2" in
    "gcc")
        CCOMPILER="gcc"
        CPPCOMPILER="g++"
        ;;

    "clang")
        CCOMPILER="clang"
        CPPCOMPILER="clang++"
        ;;
    *)
        echo "usage: make-clean.sh <Debug|Release> <gcc|clang>"
        exit 1
        ;;
esac

# Clean the old build files
if [ ! -d "./build" ]
then
    rm -rf ./build
fi
if [ ! -d "./Testing" ]
then
    rm -rf ./Testing
fi

# Make the new one
cmake -B build -DCMAKE_CXX_COMPILER=$CPPCOMPILER -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_BUILD_TYPE=$1 -S .

# Make libraries
cmake --build build --config $1

# Make library unittests
cmake --build build --config $1 --target build-unittest

# Make the samples
cmake --build build --config $1 --target samples

# Make the samples unittest
cmake --build build --config $1 --target samples-unittest

# Run the library unittests
(cd build;ctest --build-config $1 --exclude-regex sample)
