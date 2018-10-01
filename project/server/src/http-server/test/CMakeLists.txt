###############################################################################
#                               FUNCTION DEFINITIONS                          #
###############################################################################

function(addUnitTest unitTest testName testFile)
    add_test(NAME "${testName}.${unitTest}" COMMAND "${testName}" "--run_test=${unit_test}" "--catch_system_error=true")
endfunction()

function(addAllTestsIn testFile)
    get_filename_component(testName "${testFile}" NAME_WE)

    # Create executable
    add_executable("${testName}" "${testFile}" "${HTTP_SERVER_CPP_SOURCES}" "${HTTP_SERVER_HEADER_SOURCES}")
    target_include_directories("${testName}" PRIVATE "${BOOST_INCLUDE_DIRS}")
    target_link_libraries("${testName}" "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")

    # Find the subtests (the "test cases") inside the test file.
    file(READ "${testFile}" contents)
    string(REGEX MATCHALL "BOOST_AUTO_TEST_CASE *\\( *[a-zA-Z0-9_]+ *\\)" unitTests "${contents}")
    foreach(unitTest IN LISTS unitTests)
        string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" unitTest "${unitTest}")
        addUnitTest("${unitTest}" "${testName}" "${testFile}")
    endforeach()
endfunction()

###############################################################################
#                                 CMAKE SCRIPT                                #
###############################################################################

set(Boost_USE_STATIC_LIBS ON) # FindBoost.cmake uses this variable. If it's at ON, that CMake script will output the
                              # static libraries, rather than the shared libraries.
find_package(Boost REQUIRED COMPONENTS unit_test_framework) # This finds where the Boost unit test libraries and
                                                            # header files are, and sets some variables accoridngly.
set(BOOST_INCLUDE_DIRS "${boost_installation_prefix}/include")

include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

enable_testing()
foreach(testFile IN LISTS HTTP_SERVER_TEST_CPP_SOURCES)
    addAllTestsIn("${testFile}")
endforeach()
