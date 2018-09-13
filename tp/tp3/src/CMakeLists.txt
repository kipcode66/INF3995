cmake_minimum_required(VERSION 3.0.0)
project(tp3)

add_executable(tp3 main.cpp)

find_library(PISTACHE_LIB pistache "${CMAKE_SOURCE_DIR}/../lib/pistache/build/src" REQUIRED)
# Handle case not found
if ("${PISTACHE_LIB}" MATCHES ".*NOTFOUND")
	message(FATAL_ERROR "PISTACHE LIBRARY NOT FOUND")
endif()

