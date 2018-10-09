###############################################################################
#                   CMAKE SCRIPT SPECIFIC TO THE HTTP SERVER                  #
###############################################################################

set(DEPS_DIR "${CMAKE_BINARY_DIR}/dependencies")

include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

# Configure compiler
set(CMAKE_CXX_STANDARD 14)                 # Equivalent to the --std=c++<version> flag in g++
set(CMAKE_CXX_STANDARD_REQUIRED ON)        # C++ version 14 is required, not optional
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories("${DEPS_DIR}/include") # Equivalent to the -I flag in g++
link_directories("${DEPS_DIR}/lib")        # Equivalent to the -L flag in g++
link_libraries(pistache sqlite3)           # Equivalent to the -l flag in g++

# Create targets
add_executable(http-server     main.cpp "${HTTP_SERVER_HEADER_SOURCES}")
add_library   (http-server-lib          "${HTTP_SERVER_CPP_SOURCES}" "${HTTP_SERVER_HEADER_SOURCES}") # Static library used for unit tests
set_target_properties(http-server-lib PROPERTIES OUTPUT_NAME http-server)
target_link_libraries(http-server http-server-lib)

# Testing
add_subdirectory(test)
