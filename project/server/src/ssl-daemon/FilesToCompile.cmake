###############################################################################
#                    CMAKE SCRIPT SPECIFIC TO THE SSL DAEMON                  #
###############################################################################

# This CMake script contains the files that will be compiled.
# We do not use file(GLOB_RECURSE ...) because we would need to rerun CMake
# when we pull from Git and somebody added a file.

###############################################################################
#                            FUNCTION DEFINITIONS                             #
###############################################################################

# Function to transform relative paths into absolute paths so that
# the variables can be used in other directories
# Note that fileList is a VARIABLE NAME (e.g. myVar), not a value (i.e., not ${myVar})
function(makeAbsolute fileList)
    foreach(file IN LISTS "${fileList}")
        get_filename_component(fileAbsolute "${file}" ABSOLUTE)
        list(APPEND outputFileList "${fileAbsolute}")
    endforeach()
    set(${fileList} "${outputFileList}" PARENT_SCOPE)
endfunction()

###############################################################################
#                            C++ TRANSLATION UNITS                            #
###############################################################################

# All .cpp, .cc, .c files EXCEPT main.cpp
# Use quotes "" if a file contains a space (although not recommended)
set(SSL_DAEMON_CPP_SOURCES
    ArgsParser.cpp
)

###############################################################################
#                                    HEADER FILES                             #
###############################################################################

# All .h, .hpp, .hh files
# Use quotes "" if a file contains a space (although not recommended)
set(SSL_DAEMON_HEADER_SOURCES
    ArgsParser.hpp
)

makeAbsolute(SSL_DAEMON_CPP_SOURCES)
makeAbsolute(SSL_DAEMON_HEADER_SOURCES)
