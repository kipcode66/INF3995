#include "ArgsParser.hpp"

#define BOOST_TEST_MODULE ArgsParser
#include <boost/test/unit_test.hpp>

namespace elevation {
namespace daemon {

BOOST_AUTO_TEST_CASE(ArgsParserConstructor) {
    BOOST_REQUIRE_NO_THROW(ArgsParser());
}

} // namespace daemon
} // namespace elevation
