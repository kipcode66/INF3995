#!/usr/bin/env sh

CURRENT_DIR=$(pwd)
PISTACHE_BUILD_DIR=$(pwd)'/lib/pistache/build'

if [ ! -d "${PISTACHE_BUILD_DIR}" ]; then
    git submodule update --init --recursive
    mkdir -p "${PISTACHE_BUILD_DIR}"
    cd "${PISTACHE_BUILD_DIR}"
    cmake -DCMAKE_BUILD_TYPE=Debug "${PISTACHE_BUILD_DIR}/.."
    make
    cd "${CURRENT_DIR}"
fi

[ -d build ] || mkdir build
cd build
cmake ../src
make
