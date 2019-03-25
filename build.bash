#!/bin/bash

if [ "$1" = "" ]; then
    premake5 gmake && make MathernoGL
fi

if [ "$1" = "debug" ]; then
    premake5 gmake && make config=debug MathernoGL
fi

if [ "$1" = "release" ]; then
    premake5 gmake && make config=release MathernoGL
fi
