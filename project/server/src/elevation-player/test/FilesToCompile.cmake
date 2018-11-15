###############################################################################
#        CMAKE SCRIPT SPECIFIC TO THE TESTS OF THE ELEVATION PLAYER           #
###############################################################################

# This CMake script contains the files that will be compiled.
# We do not use file(GLOB_RECURSE ...) because we would need to rerun CMake
# when we pull from Git and somebody added a file.

include("${CMAKE_SCRIPTS_DIR}/FilesToCompileFunctions.cmake")

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although space in filename is not recommended)
set(ELEVATION_PLAYER_TEST_CPP_SOURCES
    TestElevationPlayerArgsParser.cpp
    mp3/player/mad/TestMadDecoder.cpp
    mp3/player/pulse/TestPulseDevice.cpp
    mp3/player/TestMp3Player.cpp
    mp3/player/TestMp3AutoPlayer.cpp
)

makeAbsolute(ELEVATION_PLAYER_TEST_CPP_SOURCES)