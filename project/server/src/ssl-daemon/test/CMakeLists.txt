include("${CMAKE_SCRIPTS_DIR}/SetupTestFunctions.cmake")
include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"
setupTests(SSL_DAEMON_TEST_CPP_SOURCES "${CMAKE_SOURCE_DIR}/ssl-daemon" ssl-daemon-lib)
