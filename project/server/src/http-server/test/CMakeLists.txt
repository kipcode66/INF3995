include("${CMAKE_SCRIPTS_DIR}/SetupTestFunctions.cmake") # Call CMake script "SetupTestFunctions.cmake"
include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"
include(FilesToGenerate.cmake) # Call CMake script "FilesToGenerate.cmake"

setupTests(HTTP_SERVER_TEST_CPP_SOURCES "${CMAKE_SOURCE_DIR}/http-server" http-server-lib)
