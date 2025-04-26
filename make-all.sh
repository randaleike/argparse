#!/bin/bash

#check the input
if [[ "$1" != "Debug" && "$1" != "Release" ]]
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

BUILD_DIR="build"
if [ "$3" != "" ]
then
    BUILD_DIR="$3"
fi

# Make the new one
cmake -B $BUILDDIR -DCMAKE_CXX_COMPILER=$CPPCOMPILER -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_BUILD_TYPE=$1 -S .

# Make libraries
cmake --build $BUILDDIR --config $1

# Make library unittests
cmake --build $BUILDDIR --config $1 --target build-unittest

# Make the samples
cmake --build $BUILDDIR --config $1 --target samples

# Run the library unittests
(cd $BUILDDIR;ctest --build-config $1 --exclude-regex sample)

# Run the samples unittests
(cd $BUILDDIR;ctest --build-config $1 --tests-regex sample)
