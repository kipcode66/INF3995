include("${CMAKE_SCRIPTS_DIR}/SetupTestFunctions.cmake") # Call CMake script "SetupTestFunctions.cmake"
include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../common/test/include") # TODO Use something more global

setupTests(ELEVATION_PLAYER_TEST_CPP_SOURCES "${CMAKE_SOURCE_DIR}/elevation-player" elevation-player-lib)
