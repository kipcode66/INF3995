###############################################################################
#          CMAKE SCRIPT SPECIFIC TO THE TESTS OF THE HTTP SERVER              #
###############################################################################

# This CMake script contains the files that will be compiled.
# We do not use file(GLOB_RECURSE ...) because we would need to rerun CMake
# when we pull from Git and somebody added a file.

include("${CMAKE_SCRIPTS_DIR}/FilesToCompileFunctions.cmake")

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although space in filename is not recommended)
set(SSL_DAEMON_TEST_CPP_SOURCES
    TestArgsParser.cpp
    IntegrationTests.cpp
)

makeAbsolute(SSL_DAEMON_TEST_CPP_SOURCES)
