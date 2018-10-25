###############################################################################
#                          VARIABLES TO CONFIGURE                             #
###############################################################################

# These variables can be used by CMake to configure template files.
# For example, if a template file contains @MY_VAR@ and we set
# the CMake variable MY_VAR to "hello", then the generated file will contain
# "hello" where there was the @MY_VAR@ in the template file.

set(HTTP_SERVER_TEST_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(TEST_MP3_PATH "${HTTP_SERVER_TEST_RESOURCE_DIR}/../test.mp3")
set(TIPPERARY_SHORT_PATH "${CMAKE_CURRENT_BINARY_DIR}/tipperary_short.mp3")

###############################################################################
#                        GENERATE FILES FROM TEMPLATES                        #
###############################################################################

set(HTTP_SERVER_TEST_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

configure_file(config/TestConfiguration.hpp.in "${HTTP_SERVER_TEST_GENERATED_INCLUDE_DIR}/TestConfiguration.hpp" @ONLY)
configure_file(test.mp3 "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
configure_file(test.txt "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
configure_file(test1.txt "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
