include("${CMAKE_SCRIPTS_DIR}/SetupTestFunctions.cmake") # Call CMake script "SetupTestFunctions.cmake"
include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

setupTests(ELEVATION_PLAYER_TEST_CPP_SOURCES "${CMAKE_SOURCE_DIR}/elevation-player" elevation-player-lib)
