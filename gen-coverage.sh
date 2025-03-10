#!/bin/bash

# check for build directories
if [ -d "./build/coverage" ]
then
    rm -rf ./build/coverage/*
fi

mkdir -p ./build/coverage

cmake --build build --config Debug --target run-unittest
gcovr --html-details ./build/coverage/coverage.html