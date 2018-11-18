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
    database/Database.cpp
    database/sqlite_error.cpp
    database/Statement.cpp
    database/Query.cpp
    os/SharedFileMemory.cpp
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
    mp3/header/Mp3Duration.hpp
    mp3/header/Mp3Header.hpp
    database/Database.hpp
    database/sqlite_error.hpp
    database/Statement.hpp
    database/Query.hpp
    database/templates/Song.hpp
    database/templates/User.hpp
    os/SharedFileMemory.hpp
    filesystem/FileCache.hpp
    misc/Base64.hpp
    misc/id_utils.hpp
)

makeAbsolute(COMMON_CPP_SOURCES)
makeAbsolute(COMMON_HEADER_SOURCES)
