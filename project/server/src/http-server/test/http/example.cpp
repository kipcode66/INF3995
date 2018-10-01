// From : https://www.boost.org/doc/libs/1_68_0/libs/test/doc/html/boost_test/testing_tools/boost_test_universal_macro.html

#define BOOST_TEST_MODULE boost_test_macro3
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( example_test_case ) {
    int a = 13, b = 12;
    BOOST_TEST(a == b);
    BOOST_TEST(a < b);
    BOOST_TEST(a - 1 < b);
    BOOST_TEST(b > a - 1);
}
