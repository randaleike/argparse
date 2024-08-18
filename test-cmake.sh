#!/bin/bash

function testbuild() {
    local buildDir=$1
    local buildType=$2
    local cCompiler=$3
    local cxxCompiler=$4

    echo "Linux $cCompiler cmake $buildType started"
    echo "Linux $cCompiler cmake $buildType\n:" >> test-cmake-linux.log
    if ! cmake -B $buildDir -DCMAKE_CXX_COMPILER=$cxxCompiler -DCMAKE_C_COMPILER=$cCompiler -DCMAKE_BUILD_TYPE=$buildType -S ./ >> test-cmake-linux.log; then
        echo "Linux $cCompiler cmake $buildType failed"
        exit 1 # cmake failed
    fi

    echo "Linux $cCompiler build $buildType started"
    echo "Linux $cCompiler build $buildType\n:" >> test-cmake-linux.log
    if ! cmake --build ./$buildDir --config $buildType >> test-cmake-linux.log; then
        echo "Linux $cCompiler build $buildType failed"
        exit 2 # build failed
    fi

    echo "Linux $cCompiler $buildType unittest started"
    echo "Linux $cCompiler $buildType unittest\n:" >> test-cmake-linux.log
    if ! make -C ./$buildDir unittest >> test-cmake-linux.log; then
        echo "Linux $cCompiler $buildType unittest failed"
        exit 3 # unittest build failed
    fi

    echo "Linux $cCompiler $buildType sample unittest started"
    echo "Linux $cCompiler $buildType sample unittest\n:" >> test-cmake-linux.log
    pushd ./$buildDir
    if ! ./sample-unittest >> ../test-cmake-linux.log; then
        popd
        echo "Linux $cCompiler $buildType sample unittest failed"
        exit 4 # unittest build failed
    fi
    popd
}

# Remove log file if it exists
if [ -f test-cmake-linux.log ]; then
    rm test-cmake-linux.log
fi

# File header
echo "Linux cmake/unittest log file\n" > test-cmake-linux.log

# Test gcc release build
testbuild "build-gcc-release" "Release" "gcc" "g++"
testbuild "build-gcc-debug" "Debug" "gcc" "g++"
testbuild "build-clang-release" "Release" "clang" "clang++"
testbuild "build-clang-debug" "Debug" "clang" "clang++"

echo "Linux cmake/unittest complete, no errors"