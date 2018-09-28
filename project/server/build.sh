#!/usr/bin/env bash

SERVER_DIR="$(git rev-parse --show-toplevel)/project/server"

buildPistache() {
    local PISTACHE_BINARY_DIR="${DEPS_DIR}/pistache/build"

    # If this file/directory exists, then the dependency is considered up-to-date.
    local PISTACHE_BUILT_FLAG="${DEPS_INSTALL_DIR}/lib/cmake/pistache"

    cd "${PISTACHE_BINARY_DIR}"
    if [ ! -e "${PISTACHE_BUILT_FLAG}" ] || [ ! -d "${PISTACHE_BINARY_DIR}" ]; then
        mkdir -p "${PISTACHE_BINARY_DIR}"
        cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="${DEPS_INSTALL_DIR}" ..
    fi
    make -j2
    make install
}

buildOpenSsl() {
    local OPEN_SSL_ROOT_DIR="${DEPS_DIR}/openssl"

    # If this file/directory exists, then the dependency is considered up-to-date.
    local OPEN_SSL_BUILT_FLAG="${DEPS_INSTALL_DIR}/lib/libssl.a"
    local OPEN_SSL_INSTALL_DIR="${DEPS_INSTALL_DIR}"
    local OPEN_SSL_CERTIFICATE_DIR="${SERVER_BINARY_DIR}/ssl"

    cd "${OPEN_SSL_ROOT_DIR}"
    if [ ! -e "${OPEN_SSL_BUILT_FLAG}" ]; then
        mkdir -p "${OPEN_SSL_CERTIFICATE_DIR}"
        ./config --prefix="${OPEN_SSL_INSTALL_DIR}" --openssldir="${OPEN_SSL_CERTIFICATE_DIR}"
        make -j2
        make install
    fi
}

buildDependencies() {
    local DEPS_DIR="${SERVER_DIR}/lib"
    local DEPS_INSTALL_DIR="${SERVER_DIR}/build/dependencies"

    git submodule update --init --recursive
    mkdir -p "${DEPS_INSTALL_DIR}"
    buildOpenSsl
    buildPistache
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