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
