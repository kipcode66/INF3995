###############################################################################
#                               FUNCTION DEFINITIONS                          #
###############################################################################

function(addUnitTest unitTest testName testFile)
    add_test(NAME "${testName}.${unitTest}" COMMAND "${testName}" "--run_test=${unitTest}*" "--catch_system_error=true")
endfunction()

function(addAllTestsIn testFile includeDirList libraryList)
    get_filename_component(testName "${testFile}" NAME_WE)

    # Create executable
    add_executable("${testName}" "${testFile}" "${includeDirList}")
    target_include_directories("${testName}" PRIVATE "${BOOST_INCLUDE_DIRS}" "${includeDirList}")
    target_link_libraries("${testName}" "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}" "${libraryList}")

    # Find the subtests (the "test cases") inside the test file.
    file(READ "${testFile}" contents)
    string(REGEX MATCHALL "BOOST_(AUTO|FIXTURE)_TEST_CASE[^\n,(]*\\( *\"?[a-zA-Z0-9_]+[^)]*\\)"   unitTests  "${contents}")
    string(REGEX MATCHALL "BOOST_TEST_SUITE[^\n]*\\( *\"?[a-zA-Z0-9_ ]+\"? *\\)" testSuites "${contents}")
    foreach(unitTest IN LISTS unitTests testSuites)
        string(REGEX REPLACE "[^\n,(]*\\( *\"?([A-Za-z_0-9]([A-Za-z_0-9 ]+[A-Za-z_0-9])?).*" "\\1" unitTest "${unitTest}")
        addUnitTest("${unitTest}" "${testName}" "${testFile}")
    endforeach()
endfunction()

macro(setupTests testSourceList includeDirList libraryList)
    set(Boost_USE_STATIC_LIBS ON) # FindBoost.cmake uses this variable. If it's at ON, that CMake script will output the
                                  # static libraries, rather than the shared libraries.
    find_package(Boost 1.65 REQUIRED
                 COMPONENTS unit_test_framework) # This finds where the Boost unit test libraries and
                                                 # header files are, and sets some variables accoridngly.
    set(BOOST_INCLUDE_DIRS "${boost_installation_prefix}/include")

    enable_testing()
    foreach(testFile IN LISTS "${testSourceList}")
        addAllTestsIn("${testFile}" "${includeDirList}" "${libraryList}")
    endforeach()
endmacro()
