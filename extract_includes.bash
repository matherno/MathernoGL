#!/bin/bash


rm -rf ./include
mkdir -p ./include
cp -r ./src/* ./include
find ./include -type f -name '*.cpp' -delete