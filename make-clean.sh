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

if [ "$3" == "" ]
then
    BUILD_DIR="build"
fi

# Clean the old build files
if [ -d "./$BUILD_DIR" ]
then
    rm -rf ./$BUILD_DIR
fi
if [ -d "./.cache" ]
then
    rm -rf ./.cache
fi
if [ -d "./Testing" ]
then
    rm -rf ./Testing
fi

# Make the new one
cmake -B $BUILD_DIR -DCMAKE_CXX_COMPILER=$CPPCOMPILER -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_BUILD_TYPE=$1 -S .

# Make libraries
cmake --build $BUILD_DIR --config $1

# Make library unittests
cmake --build $BUILD_DIR --config $1 --target build-unittest

# Make the samples
cmake --build $BUILD_DIR --config $1 --target samples

# Make the samples unittest
cmake --build $BUILD_DIR --config $1 --target samples-unittest

# Ask if we should run ctest unit test
read -r -p "Run ctest? [y/n]: " response
case "$response" in
    [yY][eE][sS]|[yY])
        # Run the library unittests
        (cd $BUILD_DIR;ctest --build-config $1 --exclude-regex sample)

        # Run the library unittests
        (cd $BUILD_DIR;ctest --build-config $1 --tests-regex sample)
        ;;
    *) ;;
esac
