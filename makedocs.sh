#!/bin/bash

# check for build
if [ ! -d "./build" ]
then
    echo "You must run cmake first"
else
    cd ./build
    echo "Build CPP API documentation"
    doxygen Doxyfile-cpp

    echo "Build C API documentation"
    doxygen Doxyfile-c
fi
