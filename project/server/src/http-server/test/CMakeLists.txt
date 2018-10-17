include("${CMAKE_SCRIPTS_DIR}/SetupTestFunctions.cmake")
include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"
setupTests(HTTP_SERVER_TEST_CPP_SOURCES "${CMAKE_SOURCE_DIR}/http-server" http-server-lib)
configure_file(test.mp3 "${CMAKE_CURRENT_BINARY_DIR}"/http-server/test COPYONLY)
configure_file(test.txt "${CMAKE_CURRENT_BINARY_DIR}"/http-server/test COPYONLY)
configure_file(test1.txt "${CMAKE_CURRENT_BINARY_DIR}"/http-server/test COPYONLY)
