# This CMake script contains the files that will be compiled by this
# sub-project. We do not use file(GLOB_RECURSE ...) because we would need to
# rerun CMake when we pull from Git and somebody added a file.

include("${CMAKE_SCRIPTS_DIR}/FilesToCompileFunctions.cmake")

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although not recommended)
set(COMMON_CPP_SOURCES
    args/RestServerArgsParser.cpp
    logger/Logger.cpp
    mp3/header/Mp3Duration.cpp
    mp3/header/Mp3Header.cpp
    mp3/event/Mp3Event.cpp
    mp3/event/VolumeChangeEvent.cpp
    mp3/event/AbstractPayloadlessEvent.cpp
    mp3/event/MuteEvent.cpp
    mp3/event/UnmuteEvent.cpp
    mp3/communication/Mp3EventSocket.cpp
    mp3/communication/Mp3EventListenerSocket.cpp
    mp3/communication/Mp3EventClientSocket.cpp
    mp3/communication/packet/Mp3EventPacketReader.cpp
    database/Database.cpp
    database/sqlite_error.cpp
    database/Statement.cpp
    database/Query.cpp
    os/SharedFileMemory.cpp
    os/Socket.cpp
    os/TestSocket.cpp
    os/ListenerSocket.cpp
    os/ClientSocket.cpp
    os/exception/SocketClosedException.cpp
    os/signal/SignalHandling.cpp
    rest/rest_utils.cpp
    filesystem/FileCache.cpp
    misc/id_utils.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(COMMON_HEADER_SOURCES
    args/RestServerArgsParser.hpp
    logger/Logger.hpp
    mp3/definitions/Volume.hpp
    mp3/header/Mp3Duration.hpp
    mp3/header/Mp3Header.hpp
    mp3/event/Mp3Event.hpp
    mp3/event/VolumeChangeEvent.hpp
    mp3/event/AbstractPayloadlessEvent.hpp
    mp3/event/MuteEvent.hpp
    mp3/event/UnmuteEvent.hpp
    mp3/event/AbstractMp3EventVisitor.hpp
    mp3/communication/Mp3EventSocket.hpp
    mp3/communication/Mp3EventListenerSocket.hpp
    mp3/communication/Mp3EventClientSocket.hpp
    mp3/communication/packet/Mp3EventPacketReader.hpp
    database/Database.hpp
    database/sqlite_error.hpp
    database/Statement.hpp
    database/Query.hpp
    database/templates/Song.hpp
    database/templates/User.hpp
    os/SharedFileMemory.hpp
    os/Socket.hpp
    os/TestSocket.hpp
    os/ListenerSocket.hpp
    os/ClientSocket.hpp
    os/exception/SocketClosedException.hpp
    os/signal/SignalHandling.hpp
    filesystem/FileCache.hpp
    rest/rest_utils.hpp
    misc/Base64.hpp
    misc/id_utils.hpp
)

makeAbsolute(COMMON_CPP_SOURCES)
makeAbsolute(COMMON_HEADER_SOURCES)
