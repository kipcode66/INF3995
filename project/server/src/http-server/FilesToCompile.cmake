###############################################################################
#                   CMAKE SCRIPT SPECIFIC TO THE HTTP SERVER                  #
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
set(HTTP_SERVER_CPP_SOURCES
    http/RestApi.cpp
    mp3-header/Mp3Duration.cpp
    mp3-header/Mp3Header.cpp
    mp3/player/Mp3Player.cpp
    mp3/player/mad/MadDecoder.cpp
    database/Database.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(HTTP_SERVER_HEADER_SOURCES
    http/RestApi.hpp
    mp3-header/Mp3Duration.hpp
    mp3-header/Mp3Header.hpp
    mp3/player/Mp3Player.hpp
    mp3/player/mad/MadDecoder.hpp
    database/Database.hpp
)

makeAbsolute(HTTP_SERVER_CPP_SOURCES)
makeAbsolute(HTTP_SERVER_HEADER_SOURCES)
