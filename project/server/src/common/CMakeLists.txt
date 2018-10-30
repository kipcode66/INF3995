###############################################################################
#                   CMAKE SCRIPT SPECIFIC TO THE HTTP SERVER                  #
###############################################################################

include(FilesToCompile.cmake ) # Call CMake script "FilesToCompile.cmake"
include(FilesToGenerate.cmake) # Call CMake script "FilesToGenerate.cmake"

# Configure compiler
set(CMAKE_CXX_STANDARD 17)                 # Equivalent to the --std=c++<version> flag in g++
set(CMAKE_CXX_STANDARD_REQUIRED ON)        # C++ version 14 is required, not optional
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")
include_directories(                       # Equivalent to the -I flag in g++
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
link_libraries(                            # Equivalent to the -l flag in g++
    stdc++fs
)

# Create targets
add_library(common "${COMMON_CPP_SOURCES}" "${COMMON_HEADER_SOURCES}")
