# This CMake script contains the files that will be compiled by this
# sub-project. We do not use file(GLOB_RECURSE ...) because we would need to
# rerun CMake when we pull from Git and somebody added a file.

include("${CMAKE_SCRIPTS_DIR}/FilesToCompileFunctions.cmake")

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although not recommended)
set(SSL_DAEMON_CPP_SOURCES
    ArgsParser.cpp
    signal/SignalHandling.cpp
    packet/HttpPacketReader.cpp
    ssl/SslContext.cpp
    ssl/SslSession.cpp
    core/DaemonRunner.cpp
    core/DaemonMaster.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(SSL_DAEMON_HEADER_SOURCES
    ArgsParser.hpp
    signal/SignalHandling.hpp
    packet/HttpPacketReader.hpp
    ssl/SslContext.hpp
    ssl/SslSession.hpp
    core/DaemonRunner.hpp
    core/DaemonMaster.hpp
)

makeAbsolute(SSL_DAEMON_CPP_SOURCES)
makeAbsolute(SSL_DAEMON_HEADER_SOURCES)
