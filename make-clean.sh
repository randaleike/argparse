#!/bin/bash

# Clean the old one
if [ ! -d "./build" ]
then
    rm -rf ./build
fi

# Make the new one
cmake -B build -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=%1 -S .

# Make libraries
cmake --build build --config %1

# Make library unittests
cmake --build build --config %1 --target build-unittest

# Make the samples
cmake --build build --config %1 --target samples

# Make the samples unittest
cmake --build build --config %1 --target samples-unittest

# Run the library unittests
cd build;ctest --build-config %1 --exclude-regex samples
