include("${CMAKE_SCRIPTS_DIR}/SetupTestFunctions.cmake")
include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"
setupTests(HTTP_SERVER_TEST_CPP_SOURCES "${CMAKE_SOURCE_DIR}/http-server" http-server-lib)
