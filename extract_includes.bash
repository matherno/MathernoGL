#!/bin/bash


rm -rf ./include
mkdir -p ./include
cp -r ./src/* ./include
find ./include -type f -name '*.cpp' -delete

cp ./bin_release/libMathernoGL.a ./include
cp ./bin_debug/libMathernoGL.a ./include/libMathernoGL_DEBUG.a