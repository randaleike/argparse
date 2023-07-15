#!/bin/bash

# check for build
if [ ! -d "./build-release" ] 
then 
    mkdir ./build-release
fi
if [ ! -d "./build-debug" ] 
then 
    mkdir ./build-debug
fi


# Create the make files
cd ./build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..

cd ./build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd ..