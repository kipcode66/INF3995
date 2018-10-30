###############################################################################
#                          VARIABLES TO CONFIGURE                             #
###############################################################################

# These variables can be used by CMake to configure template files.
# For example, if a template file contains @MY_VAR@ and we set
# the CMake variable MY_VAR to "hello", then the generated file will contain
# "hello" where there was the @MY_VAR@ in the template file.

set(SERVER_LOG_DIR "${CMAKE_BINARY_DIR}/../logs")

###############################################################################
#                        GENERATE FILES FROM TEMPLATES                        #
###############################################################################

set(COMMON_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

configure_file(config/Config.hpp.in "${COMMON_GENERATED_INCLUDE_DIR}/elevation/common/Config.hpp" @ONLY)
