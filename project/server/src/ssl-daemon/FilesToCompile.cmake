###############################################################################
#                    CMAKE SCRIPT SPECIFIC TO THE SSL DAEMON                  #
###############################################################################

# This CMake script contains the files that will be compiled.
# We do not use file(GLOB_RECURSE ...) because we would need to rerun CMake
# when we pull from Git and somebody added a file.

include("${CMAKE_SCRIPTS_DIR}/FilesToCompileFunctions.cmake")

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although not recommended)
set(SSL_DAEMON_CPP_SOURCES
    ArgsParser.cpp
    signal/SignalHandling.cpp
    communication/Socket.cpp
    communication/LocalSocket.cpp
    communication/IpSocket.cpp
    communication/ListenerSocket.cpp
    ssl/SslContext.cpp
    ssl/SslSession.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(SSL_DAEMON_HEADER_SOURCES
    ArgsParser.hpp
    signal/SignalHandling.hpp
    communication/Socket.hpp
    communication/LocalSocket.hpp
    communication/IpSocket.hpp
    communication/ListenerSocket.hpp
    ssl/SslContext.hpp
    ssl/SslSession.hpp
)

makeAbsolute(SSL_DAEMON_CPP_SOURCES)
makeAbsolute(SSL_DAEMON_HEADER_SOURCES)
