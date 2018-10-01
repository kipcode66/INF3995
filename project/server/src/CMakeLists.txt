cmake_minimum_required(VERSION 3.1.0)
project(elevation_server)

add_subdirectory(http-server) # Call CMakeLists.txt inside 'http-server/'
add_subdirectory(ssl-daemon ) # Call CMakeLists.txt inside 'ssl-daemon/'
