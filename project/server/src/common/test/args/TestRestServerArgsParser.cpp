#include "args/RestServerArgsParser.hpp"

#define BOOST_TEST_MODULE RestServerArgsParser
#include <boost/test/unit_test.hpp>

#include <stdexcept>

namespace elevation {

BOOST_AUTO_TEST_CASE(parseArgsSuccess) {
    struct TestCase {
        std::vector<std::string> input;
        uint32_t expectedPort;
        uint32_t expectedPlayerPort;
        std::string expectedCachePath;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({{"./test", "-p"           , "4444", "-c", "/tmp", "-l", "6666"}, 4444, 6666, "/tmp"});
    testCases.push_back({{"./test", "-l"           , "6666", "-c", "/foo", "-p", "1234"}, 1234, 6666, "/foo"});
    testCases.push_back({{"./test", "--port"       , "4444", "-c", "/tmp", "-l", "6666"}, 4444, 6666, "/tmp"});
    testCases.push_back({{"./test", "--cache-path" , "/foo", "-l", "6666", "-p", "1234"}, 1234, 6666, "/foo"});
    testCases.push_back({{"./test", "--player-port", "6789", "-c", "/foo", "-p", "1234"}, 1234, 6789, "/foo"});

    for (const auto& testCase : testCases) {
        RestServerArgsParser argsParser{testCase.input};
        BOOST_TEST(argsParser.getPort()       == testCase.expectedPort);
        BOOST_TEST(argsParser.getPlayerPort() == testCase.expectedPlayerPort);
        BOOST_TEST(argsParser.getCachePath()  == testCase.expectedCachePath);
    }
}

BOOST_AUTO_TEST_CASE(parseArgsFailure) {
    struct TestCase {
        std::vector<std::string> input;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({{"./test", "-p", "-c", "1234"        }});
    testCases.push_back({{"./test", "-c", "-p", "4444"        }});
    testCases.push_back({{"./test", "-p", "4444", "-c"        }});
    testCases.push_back({{"./test", "-c", "1234", "-p"        }});
    testCases.push_back({{"./test", "-c", "1234", "-l"        }});
    testCases.push_back({{"./test", "-p", "-c"                }});
    testCases.push_back({{"./test", "-c"                      }});
    testCases.push_back({{"./test", "-p"                      }});
    testCases.push_back({{"./test", "-l"                      }});
    testCases.push_back({{"./test", "-X"                      }});
    testCases.push_back({{"./test", "-X", "1234", "-Y", "/tmp"}});
    testCases.push_back({{"./test", "1234"                    }});
    testCases.push_back({{"./test", "1234", "4444"            }});

    for (const auto& testCase : testCases) {
        BOOST_CHECK_THROW(RestServerArgsParser argsParser{testCase.input}, std::invalid_argument);
    }
}

} // namespace elevation
