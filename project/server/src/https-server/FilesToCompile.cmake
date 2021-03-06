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
    https/descriptions/VolumeApi.cpp
    https/descriptions/BlacklistApi.cpp
    https/descriptions/StatsApi.cpp
    https/descriptions/AuthApi.cpp
    https/descriptions/FileManagementApi.cpp
    https/exception/AuthenticationFailureException.cpp
    mp3/event/HttpsServerMp3EventVisitor.cpp
    mp3/event/HttpsServerEventManager.cpp
    mp3/event/HttpsServerVolumeGetRequestAdapter.cpp
    mp3/event/HttpsServerEventFacade.cpp
    Admin.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(HTTPS_SERVER_HEADER_SOURCES
    https/SecureRestApi.hpp
    https/descriptions/VolumeApi.hpp
    https/descriptions/BlacklistApi.hpp
    https/descriptions/StatsApi.hpp
    https/descriptions/AuthApi.hpp
    https/descriptions/FileManagementApi.hpp
    https/exception/AuthenticationFailureException.hpp
    mp3/event/HttpsServerMp3EventVisitor.hpp
    mp3/event/HttpsServerEventManager.hpp
    mp3/event/HttpsServerVolumeGetRequestAdapter.hpp
    mp3/event/HttpsServerEventFacade.hpp
    Admin.hpp
)

makeAbsolute(HTTPS_SERVER_CPP_SOURCES)
makeAbsolute(HTTPS_SERVER_HEADER_SOURCES)

