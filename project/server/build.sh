#!/usr/bin/env bash

SERVER_DIR="$(git rev-parse --show-toplevel)/project/server"

buildDependencies() {
    local DEPS_DIR="${SERVER_DIR}/lib"
    local DEPS_INSTALL_DIR="${SERVER_DIR}/build/dependencies"
    local PISTACHE_BINARY_DIR="${DEPS_DIR}/pistache/build"
    local PISTACHE_BUILT_FLAG="${DEPS_INSTALL_DIR}/lib/cmake/pistache"

    if [ ! -e "${PISTACHE_BUILT_FLAG}" ] || [ ! -d "${PISTACHE_BINARY_DIR}" ]; then
        git submodule update --init --recursive
        mkdir -p "${PISTACHE_BINARY_DIR}"
        cd "${PISTACHE_BINARY_DIR}"
        cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="${DEPS_INSTALL_DIR}" ..
    fi

    cd "${PISTACHE_BINARY_DIR}"
    make
    make install
}

buildServer() {
    local SERVER_BINARY_DIR="${SERVER_DIR}/build"
    mkdir -p "${SERVER_BINARY_DIR}"

    buildDependencies
    cd "${SERVER_BINARY_DIR}"
    cmake -DCMAKE_BUILD_TYPE=Debug ../src
    make
}

buildServer
