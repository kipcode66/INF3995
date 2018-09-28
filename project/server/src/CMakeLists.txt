cmake_minimum_required(VERSION 3.0.0)
project(gottaFindAName)

set(DEPS_DIR "${CMAKE_BINARY_DIR}/dependencies")

add_executable(httpServer main.cpp)

function(useDependency dependencyName)
    include_directories("${DEPS_DIR}/include/{dependencyName}")
endfunction()

link_directories("${DEPS_DIR}/lib")
useDependency(pistache)
useDependency(openssl)
