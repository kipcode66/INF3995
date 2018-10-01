set(Boost_USE_STATIC_LIBS ON) # FindBoost.cmake uses this variable. If it's at ON, that CMake script will output the
                              # static libraries, rather than the shared libraries.
find_package(Boost REQUIRED COMPONENTS unit_test_framework) # This finds where the Boost unit test libraries and
                                                            # header files are, and sets some variables accoridngly.
set(BOOST_INCLUDE_DIRS "${boost_installation_prefix}/include")

include(FilesToCompile.cmake) # Call CMake script "FilesToCompile.cmake"

enable_testing()
foreach(testFile ${HTTP_SERVER_TEST_CPP_SOURCES})
    get_filename_component(testName "${testFile}" NAME_WE)
    add_executable("${testName}" "${testFile}" "${HTTP_SERVER_CPP_SOURCES}" "${HTTP_SERVER_HEADER_SOURCES}")
    target_include_directories("${testName}" PRIVATE "${BOOST_INCLUDE_DIRS}")
    target_link_libraries("${testName}" "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
    add_test(NAME ${testName} COMMAND "${testName}")
endforeach()
