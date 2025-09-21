#!/bin/bash

# check for build directories
if [ -d "./build/coverage" ]
then
    rm -rf ./build/coverage/*
fi

mkdir -p ./build/coverage

./run-unittest.sh build
gcovr --html-details ./build/coverage/coverage.html -e 'build/_deps/*' --gcov-exclude 'mock_*.*' -r ./
