###############################################################################
#                          VARIABLES TO CONFIGURE                             #
###############################################################################

# These variables can be used by CMake to configure template files.
# For example, if a template file contains @MY_VAR@ and we set
# the CMake variable MY_VAR to "hello", then the generated file will contain
# "hello" where there was the @MY_VAR@ in the template file.

set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TEST_MP3_PATH "${RESOURCE_DIR}/mp3/test.mp3")
set(TEST_TEXT_PATH "${RESOURCE_DIR}/mp3/test.txt")
set(TEST_MP3_AS_TXT_PATH "${RESOURCE_DIR}/mp3/test1.txt")
set(TEST_TXT_AS_MP3_PATH "${RESOURCE_DIR}/mp3/test1.mp3")
set(TIPPERARY_PATH "${RESOURCE_DIR}/mp3/tipperary.mp3")
set(TIPPERARY_SHORT_PATH "${RESOURCE_DIR}/mp3/tipperary_short.mp3")
set(TIPPERARY_SHORT_WAV_PATH "${RESOURCE_DIR}/mp3/tipperary_short.wav")
set(DATABASE_TEST_PATH "${RESOURCE_DIR}/database/testServer.db")

set(FILE_CACHE_TEST_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/file_cache")

###############################################################################
#                        GENERATE FILES FROM TEMPLATES                        #
###############################################################################

set(TEST_RESOURCES_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

configure_file(config/TestConfiguration.hpp.in "${TEST_RESOURCES_GENERATED_INCLUDE_DIR}/TestResources.hpp" @ONLY)
