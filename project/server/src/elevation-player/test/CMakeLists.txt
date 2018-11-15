include("${CMAKE_SCRIPTS_DIR}/SetupTestFunctions.cmake") # Call CMake script "SetupTestFunctions.cmake"
include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

include_directories("${TEST_RESOURCES_INCLUDE_DIR}")

setupTests(ELEVATION_PLAYER_TEST_CPP_SOURCES "${CMAKE_SOURCE_DIR}/elevation-player" elevation-player-lib)
