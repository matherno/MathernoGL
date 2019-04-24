#!/bin/bash

premake5 gmake && make config=debug MathernoGL && make config=release MathernoGL

