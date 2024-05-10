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
if [ -d "./build" ] 
then 
    rm -rf ./build
fi
if [ -d ".cache" ] 
then 
    rm -rf .cache
fi