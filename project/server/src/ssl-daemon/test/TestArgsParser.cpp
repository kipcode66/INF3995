#include "ArgsParser.hpp"

#define BOOST_TEST_MODULE ArgsParser
#include <boost/test/unit_test.hpp>

#include <stdexcept>

BOOST_TEST_DONT_PRINT_LOG_VALUE(elevation::daemon::ArgsParser::Config);

namespace elevation {
namespace daemon {

BOOST_AUTO_TEST_CASE(constructor) {
    BOOST_REQUIRE_NO_THROW(ArgsParser());
}

BOOST_AUTO_TEST_CASE(parseArgsSuccess) {
    struct TestCase {
        std::vector<std::string> input;
        ArgsParser::Config expectedOutput;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({{"-l", "4444", "-o", "1234"}, ArgsParser::Config(4444, 1234)});
    testCases.push_back({{"-o", "1234", "-l", "4444"}, ArgsParser::Config(4444, 1234)});
    testCases.push_back({{"-l", "4444"              }, ArgsParser::Config(4444, 7777)});
    testCases.push_back({{"-o", "1234"              }, ArgsParser::Config(443 , 1234)});
    testCases.push_back({{"-l", "4444"              }, ArgsParser::Config(4444, 7777)});
    testCases.push_back({{                          }, ArgsParser::Config(443 , 7777)});
    testCases.push_back({{"-l", "3333", "-l", "4444"}, ArgsParser::Config(4444 , 7777)});

    for (const auto& testCase : testCases) {
        ArgsParser argsParser;
        ArgsParser::Config config = argsParser.parseArgs(testCase.input);
        BOOST_TEST(config == testCase.expectedOutput);
    }
}

BOOST_AUTO_TEST_CASE(parseArgsFailure) {
    struct TestCase {
        std::vector<std::string> input;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({{"-l", "-o", "1234"}});
    testCases.push_back({{"-o", "-l", "4444"}});
    testCases.push_back({{"-l", "4444", "-o"}});
    testCases.push_back({{"-o", "1234", "-l"}});
    testCases.push_back({{"-l", "-o"        }});
    testCases.push_back({{"-o"              }});
    testCases.push_back({{"-l"              }});
    testCases.push_back({{"-X"              }});
    testCases.push_back({{"-X", "1234"      }});
    testCases.push_back({{"1234"            }});
    testCases.push_back({{"1234", "4444"    }});

    for (const auto& testCase : testCases) {
        ArgsParser argsParser;
        BOOST_CHECK_THROW(argsParser.parseArgs(testCase.input), std::invalid_argument);
    }
}

} // namespace daemon
} // namespace elevation
