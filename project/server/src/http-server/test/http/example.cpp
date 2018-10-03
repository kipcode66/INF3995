// From : https://www.boost.org/doc/libs/1_68_0/libs/test/doc/html/boost_test/testing_tools/boost_test_universal_macro.html

#define BOOST_TEST_MODULE EXAMPLE
#include <boost/test/unit_test.hpp>

#include "http/RequestHandler.hpp"

namespace elevation {
namespace http {

BOOST_AUTO_TEST_CASE(passing_test_case) {
    RequestHandler requestHandler = RequestHandler();
    BOOST_TEST(1 == 1);
}

BOOST_AUTO_TEST_CASE(failing_test_case) {
    BOOST_TEST(1 == 2);
}

} // namespace http
} // namespace elevation
