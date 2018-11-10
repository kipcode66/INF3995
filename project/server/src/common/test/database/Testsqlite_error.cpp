#define BOOST_TEST_MODULE sqlite_error
#include <boost/test/unit_test.hpp>

#include <database/sqlite_error.hpp>
#include <sqlite3/sqlite3.h>

#include <TestConfiguration.hpp>

namespace elevation {

BOOST_AUTO_TEST_CASE(constructionDestruction) {
    sqlite_error error1{SQLITE_OK};
    sqlite_error error2{SQLITE_ROW};
    sqlite_error error3{SQLITE_DONE};
    sqlite_error error4{SQLITE_ERROR};
}

BOOST_AUTO_TEST_CASE(copyConstructor) {
    sqlite_error errorOk{SQLITE_OK};
    sqlite_error errorError{SQLITE_ERROR};
    sqlite_error copiedOk = errorOk;
    sqlite_error copiedError = errorError;
    BOOST_TEST(errorOk   .what() == copiedOk   .what());
    BOOST_TEST(errorError.what() == copiedError.what());
}

BOOST_AUTO_TEST_CASE(copyAssignment) {
    sqlite_error errorOk    {SQLITE_OK};
    sqlite_error errorError {SQLITE_ERROR};
    sqlite_error copiedOk   {SQLITE_ERROR};
    sqlite_error copiedError{SQLITE_OK};
    copiedOk = errorOk;
    copiedError = errorError;
    BOOST_TEST(errorOk   .what() == copiedOk   .what());
    BOOST_TEST(errorError.what() == copiedError.what());
}

BOOST_AUTO_TEST_CASE(specificMessage) {
    const std::string OK_MSG = "Everything is awesome.";
    const std::string ERROR_MSG = "Chuck Norris disapproves.";
    sqlite_error errorOk{SQLITE_OK, OK_MSG};
    sqlite_error errorError{SQLITE_ERROR, ERROR_MSG};
    BOOST_TEST(errorOk   .what() == OK_MSG);
    BOOST_TEST(errorError.what() == ERROR_MSG);
}

} // namespace elevation
