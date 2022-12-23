#!/bin/bash

# check for build
if [ ! -d "./build" ] 
then 
    mkdir ./build
fi

# start the build
cd ./build
cmake ..
cd ..