cmake_minimum_required(VERSION 3.1.0)
project(elevation_server)

set(CMAKE_SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(http-server) # Call CMakeLists.txt inside 'http-server/'
add_subdirectory(ssl-daemon ) # Call CMakeLists.txt inside 'ssl-daemon/'
