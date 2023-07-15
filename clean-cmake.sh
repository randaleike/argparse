#!/bin/bash

# check for build
if [ -d "./build-release" ] 
then 
    rm -rf ./build-release
fi
if [ -d "./build-debug" ] 
then 
    rm -rf ./build-debug
fi