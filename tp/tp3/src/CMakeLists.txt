cmake_minimum_required(VERSION 3.0.0)
project(tp3)

# FIND LIBRARIES

set(PISTACHE_DIR "${CMAKE_SOURCE_DIR}/../lib/pistache/build/src")

find_library(PISTACHE_LIB pistache "${PISTACHE_DIR}")
# Handle case not found
if ("${PISTACHE_LIB}" MATCHES ".*NOTFOUND")
	message(FATAL_ERROR "PISTACHE LIBRARY NOT FOUND")
endif()

# ADD EXECUTABLES
add_executable(tp3 "main.cpp")
target_link_libraries(tp3 "${PISTACHE_LIB}")
target_include_directories(tp3 PUBLIC "${PISTACHE_DIR}")
