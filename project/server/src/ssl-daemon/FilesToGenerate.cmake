###############################################################################
#                          VARIABLES TO CONFIGURE                             #
###############################################################################

# These variables can be used by CMake to configure template files.
# For example, if a template file contains @MY_VAR@ and we set
# the CMake variable MY_VAR to "hello", then the generated file will contain
# "hello" where there was the @MY_VAR@ in the template file.

set(SSL_DAEMON_CERTIFICATE "${SSL_RESOURCE_DIR}/elevd-cert.pem")
set(SSL_DAEMON_PRIVATE_KEYFILE "${SSL_RESOURCE_DIR}/elevd-pkey.pem")

###############################################################################
#                        GENERATE FILES FROM TEMPLATES                        #
###############################################################################

set(SSL_DAEMON_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

configure_file(config/Config.hpp.in "${SSL_DAEMON_GENERATED_INCLUDE_DIR}/elevation/daemon/Config.hpp")

