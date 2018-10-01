###############################################################################
#          CMAKE SCRIPT SPECIFIC TO THE TESTS OF THE HTTP SERVER              #
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
# Use quotes "" if a file contains a space (although space in filename is not recommended)
set(HTTP_SERVER_TEST_CPP_SOURCES
    http/example.cpp
    http/example2.cpp
)

makeAbsolute(HTTP_SERVER_TEST_CPP_SOURCES)
