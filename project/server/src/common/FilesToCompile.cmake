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
    logger/Logger.cpp
    mp3/header/Mp3Duration.cpp
    mp3/header/Mp3Header.cpp
    mp3/player/Mp3Player.cpp
    mp3/player/mad/MadDecoder.cpp
    mp3/player/mad/MadAudioFormatter.cpp
    mp3/player/pulse/PulseDevice.cpp
    database/Database.cpp
    os/SharedFileMemory.cpp
    filesystem/FileCache.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(COMMON_HEADER_SOURCES
    logger/Logger.hpp
    mp3/header/Mp3Duration.hpp
    mp3/header/Mp3Header.hpp
    mp3/player/Mp3Player.hpp
    mp3/player/mad/MadDecoder.hpp
    mp3/player/pulse/PulseDevice.hpp
    mp3/player/mad/MadAudioFormatter.hpp
    database/Database.hpp
    os/SharedFileMemory.hpp
    filesystem/FileCache.hpp
    misc/Base64.hpp
)

makeAbsolute(COMMON_CPP_SOURCES)
makeAbsolute(COMMON_HEADER_SOURCES)
