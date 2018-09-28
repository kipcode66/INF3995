cmake_minimum_required(VERSION 3.1.0)
project(elevation_server)

set(DEPS_DIR "${CMAKE_BINARY_DIR}/dependencies")

include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

# Configure compiler
include_directories("${DEPS_DIR}/include") # Equivalent to the -I in g++
link_directories("${DEPS_DIR}/lib")        # Equivalent to the -L flag in g++
link_libraries(pistache)                   # Equivalent to the -l flag in g++
set(CMAKE_CXX_STANDARD 14)                 # Equivalent to the --std=c++<version> flag in g++
set(CMAKE_CXX_STANDARD_REQUIRED ON)        # C++ version 14 is required, not optional
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Create targets
add_executable(server     main.cpp "${SERVER_CPP_SOURCES}" "${SERVER_HEADER_SOURCES}")
add_library   (server-lib          "${SERVER_CPP_SOURCES}" "${SERVER_HEADER_SOURCES}") # Static library used for unit tests
set_target_properties(server-lib PROPERTIES OUTPUT_NAME server)
