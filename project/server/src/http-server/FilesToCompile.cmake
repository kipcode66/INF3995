# This CMake script contains the files that will be compiled by this
# sub-project. We do not use file(GLOB_RECURSE ...) because we would need to
# rerun CMake when we pull from Git and somebody added a file.

include("${CMAKE_SCRIPTS_DIR}/FilesToCompileFunctions.cmake")

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although not recommended)
set(HTTP_SERVER_CPP_SOURCES
    http/exception/BadRequestException.cpp
    http/exception/MissingTokenException.cpp
    http/exception/InvalidTokenException.cpp
    http/RestApi.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(HTTP_SERVER_HEADER_SOURCES
    http/exception/BadRequestException.hpp
    http/exception/MissingTokenException.hpp
    http/exception/InvalidTokenException.hpp
    http/RestApi.hpp
)

makeAbsolute(HTTP_SERVER_CPP_SOURCES)
makeAbsolute(HTTP_SERVER_HEADER_SOURCES)
