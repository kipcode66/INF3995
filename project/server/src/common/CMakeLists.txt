set(PRIVATE_SOCKETS_DIR "${CMAKE_CURRENT_BINARY_DIR}/private-sockets")
configure_file(src/common.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/include/elevation/common.hpp" @ONLY) # Replaces @PRIVATE_SOCKETS_DIR@ by its value, and puts
                                                                                                   # the result file somewhere in thie 'build/' directory.
