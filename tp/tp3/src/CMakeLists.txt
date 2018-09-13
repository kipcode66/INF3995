cmake_minimum_required(VERSION 3.0.0)
project(tp3)

# GENERAL CONFIG

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


# SOURCES

set(MY_CXX_SOURCES "main.cpp" "HelloHandler.cpp" "HelloHandler.hpp")


# FIND LIBRARIES

set(PISTACHE_DIR "${CMAKE_SOURCE_DIR}/../lib/pistache")

find_library(PISTACHE_LIB pistache "${PISTACHE_DIR}/build/src")
# Handle case not found
if ("${PISTACHE_LIB}" MATCHES ".*NOTFOUND")
	message(FATAL_ERROR "PISTACHE LIBRARY NOT FOUND")
endif()

# ADD EXECUTABLES
add_executable(tp3 ${MY_CXX_SOURCES})
target_link_libraries(tp3 "${PISTACHE_LIB}")
target_include_directories(tp3 PUBLIC "${PISTACHE_DIR}/include")
