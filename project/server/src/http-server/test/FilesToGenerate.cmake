###############################################################################
#                          VARIABLES TO CONFIGURE                             #
###############################################################################

# These variables can be used by CMake to configure template files.
# For example, if a template file contains @MY_VAR@ and we set
# the CMake variable MY_VAR to "hello", then the generated file will contain
# "hello" where there was the @MY_VAR@ in the template file.

set(HTTP_SERVER_TEST_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(TEST_MP3_PATH "${HTTP_SERVER_TEST_RESOURCE_DIR}/mp3/test.mp3")
set(TEST_TEXT_PATH "${HTTP_SERVER_TEST_RESOURCE_DIR}/mp3/test.txt")
set(TEST_MP3_AS_TXT_PATH "${HTTP_SERVER_TEST_RESOURCE_DIR}/mp3/test1.txt")
set(TIPPERARY_PATH "${HTTP_SERVER_TEST_RESOURCE_DIR}/mp3/tipperary.mp3")
set(TIPPERARY_SHORT_PATH "${HTTP_SERVER_TEST_RESOURCE_DIR}/mp3/tipperary_short.mp3")
set(TIPPERARY_SHORT_WAV_PATH "${HTTP_SERVER_TEST_RESOURCE_DIR}/mp3/tipperary_short.wav")

set(FILE_CACHE_TEST_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/file_cache")

###############################################################################
#                        GENERATE FILES FROM TEMPLATES                        #
###############################################################################

set(HTTP_SERVER_TEST_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

configure_file(config/TestConfiguration.hpp.in "${HTTP_SERVER_TEST_GENERATED_INCLUDE_DIR}/TestConfiguration.hpp" @ONLY)
