cmake_minimum_required(VERSION 3.1.0)
project(elevation_server)

set(CMAKE_SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/cmake") # NOTE : This variable is not a special CMake variable ;
                                                   # I just decided to call this variable like this.
set(COMMON_INCLUDE_DIR "${CMAKE_BINARY_DIR}/common/include")
set(SSL_RESOURCE_DIR "${CMAKE_SOURCE_DIR}/../res/ssl")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/common/include")

add_subdirectory(common      ) # Call CMakeLists.txt inside 'common/'
add_subdirectory(http-server ) # Call CMakeLists.txt inside 'http-server/'
add_subdirectory(https-server) # Call CMakeLists.txt inside 'https-server/'
add_subdirectory(ssl-daemon  ) # Call CMakeLists.txt inside 'ssl-daemon/'
