# This CMake script contains the files that will be compiled by this
# sub-project. We do not use file(GLOB_RECURSE ...) because we would need to
# rerun CMake when we pull from Git and somebody added a file.

include("${CMAKE_SCRIPTS_DIR}/FilesToCompileFunctions.cmake")

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although not recommended)
set(ELEVATION_PLAYER_CPP_SOURCES
    ArgsParser.cpp
    mp3/player/Mp3AutoPlayer.cpp
    mp3/player/Mp3Player.cpp
    mp3/player/mad/MadDecoder.cpp
    mp3/player/pulse/PulseDevice.cpp
    mp3/player/pulse/PulseOperation.cpp
    mp3/player/pulse/PulseVolume.cpp
    mp3/player/mad/MadAudioFormatter.cpp
    mp3/event/ElevationPlayerMp3EventVisitor.cpp
    mp3/event/ElevationPlayerEventManager.cpp
    mp3/Mp3AutoPlayerCallbacks.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(ELEVATION_PLAYER_HEADER_SOURCES
    ArgsParser.hpp
    mp3/player/Mp3AutoPlayer.hpp
    mp3/player/Mp3Player.hpp
    mp3/player/mad/MadDecoder.hpp
    mp3/player/pulse/PulseDevice.hpp
    mp3/player/pulse/PulseOperation.hpp
    mp3/player/pulse/PulseVolume.hpp
    mp3/player/mad/MadAudioFormatter.hpp
    mp3/event/ElevationPlayerMp3EventVisitor.hpp
    mp3/event/ElevationPlayerEventManager.hpp
    mp3/Mp3AutoPlayerCallbacks.hpp
)

makeAbsolute(ELEVATION_PLAYER_CPP_SOURCES)
makeAbsolute(ELEVATION_PLAYER_HEADER_SOURCES)
