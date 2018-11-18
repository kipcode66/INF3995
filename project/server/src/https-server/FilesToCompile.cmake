###############################################################################
#                   CMAKE SCRIPT SPECIFIC TO THE HTTPS SERVER                  #
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
set(HTTPS_SERVER_CPP_SOURCES
    https/SecureRestApi.cpp
    https/volume.cpp
    https/blacklist.cpp
    https/statistics.cpp
    https/auth.cpp
    https/statistics.cpp
    https/fileManagement.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(HTTPS_SERVER_HEADER_SOURCES
    https/SecureRestApi.hpp
    https/Admin.hpp
)

makeAbsolute(HTTPS_SERVER_CPP_SOURCES)
makeAbsolute(HTTPS_SERVER_HEADER_SOURCES)
