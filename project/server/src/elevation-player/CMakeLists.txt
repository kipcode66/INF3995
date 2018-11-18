###############################################################################
#                CMAKE SCRIPT SPECIFIC TO THE ELEVATION PLAYER                #
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
    sqlite3
    pulse
    pulse-simple
    mad
    stdc++fs
)

# Create targets
add_executable(elevation-player     main.cpp "${ELEVATION_PLAYER_HEADER_SOURCES}")
add_library   (elevation-player-lib          "${ELEVATION_PLAYER_CPP_SOURCES}" "${ELEVATION_PLAYER_HEADER_SOURCES}") # Static library used for unit tests
set_target_properties(elevation-player-lib PROPERTIES OUTPUT_NAME elevation-player)
target_link_libraries(elevation-player elevation-player-lib)

# Testing
add_subdirectory(test)
