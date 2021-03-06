###############################################################################
#                   CMAKE SCRIPT SPECIFIC TO THE HTTP SERVER                  #
###############################################################################

set(DEPS_DIR "${CMAKE_BINARY_DIR}/dependencies")

include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

# Configure compiler
set(CMAKE_CXX_STANDARD 17)                 # Equivalent to the --std=c++<version> flag in g++
set(CMAKE_CXX_STANDARD_REQUIRED ON)        # C++ version 14 is required, not optional
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")
include_directories(                       # Equivalent to the -I flag in g++
    "${DEPS_DIR}/include"
    "${COMMON_INCLUDE_DIR}"
    "${COMMON_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
link_directories("${DEPS_DIR}/lib")        # Equivalent to the -L flag in g++
link_libraries(                            # Equivalent to the -l flag in g++
    common
    pistache
    sqlite3
    http-server-lib
)

# Create targets
add_executable(https-server     main.cpp "${HTTPS_SERVER_HEADER_SOURCES}")
add_library   (https-server-lib          "${HTTPS_SERVER_CPP_SOURCES}" "${HTTPS_SERVER_HEADER_SOURCES}") # Static library used for unit tests
set_target_properties(https-server-lib PROPERTIES OUTPUT_NAME https-server)
target_link_libraries(https-server https-server-lib)

