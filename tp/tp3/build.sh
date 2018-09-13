#!/usr/bin/env sh
[ -d build ] || mkdir build
cd build
cmake ../src
make
