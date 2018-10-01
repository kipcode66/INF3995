###############################################################################
#                    CMAKE SCRIPT SPECIFIC TO THE SSL DAEMON                  #
###############################################################################

set(DEPS_DIR "${CMAKE_BINARY_DIR}/dependencies")

include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

# Configure compiler
set(CMAKE_CXX_STANDARD 14)                 # Equivalent to the --std=c++<version> flag in g++
set(CMAKE_CXX_STANDARD_REQUIRED ON)        # C++ version 14 is required, not optional
include_directories("${DEPS_DIR}/include") # Equivalent to the -I flag in g++
link_directories("${DEPS_DIR}/lib")        # Equivalent to the -L flag in g++
link_libraries(ssl)                        # Equivalent to the -l flag in g++

# Create targets
add_executable(ssl-daemon     main.cpp "${SSL_DAEMON_CPP_SOURCES}" "${SSL_DAEMON_HEADER_SOURCES}")
add_library   (ssl-daemon-lib          "${SSL_DAEMON_CPP_SOURCES}" "${SSL_DAEMON_HEADER_SOURCES}") # Static library used for unit tests
set_target_properties(ssl-daemon-lib PROPERTIES OUTPUT_NAME ssl-daemon)
