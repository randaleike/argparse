#!/bin/bash

if [ "$1" == "" ]
then
    echo "usage: makedocs.sh <cmake build dir>"
    exit 1
fi

# check for build
if [ ! -d "./$1" ]
then
    echo "You must run 'cmake -B <build dir>' first to generate the doxygen input files"
    exit 2
fi

cd ./$1
echo "Build CPP API documentation"
doxygen Doxyfile-cpp

echo "Build C API documentation"
doxygen Doxyfile-c
