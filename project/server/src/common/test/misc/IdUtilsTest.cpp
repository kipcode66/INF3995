#define BOOST_TEST_MODULE sqlite_error
#include <boost/test/unit_test.hpp>
#include <typeinfo>
#include <stdint.h>

#include <database/sqlite_error.hpp>
#include <sqlite3/sqlite3.h>
#include <misc/id_utils.hpp>

#include <TestResources.hpp>

namespace elevation {
const int numberOfTestCases = 100;
const int macAdressLength = 17;
const int md5HashLength = 32;
const std::string macToHash = "11:22:33:44:55:66";


BOOST_AUTO_TEST_CASE(saltLength) {
    for (int i = 1; i <= numberOfTestCases; i++) {
        BOOST_CHECK_EQUAL(id_utils::generateSalt(i).length(), i);
    }
}

BOOST_AUTO_TEST_CASE(saltType) {
    for (int i = 1; i <= numberOfTestCases; i++) {
        BOOST_CHECK_EQUAL(typeid(id_utils::generateSalt(i)).name(), typeid(std::string).name());
    }
}

BOOST_AUTO_TEST_CASE(md5Length) {
    for (int i = 0; i < numberOfTestCases; i++) {
        BOOST_CHECK_EQUAL(id_utils::generateMd5Hash(macToHash, id_utils::generateSalt(i)).length(), md5HashLength);
    }
}

BOOST_AUTO_TEST_CASE(md5Type) {
    for (int i = 0; i < numberOfTestCases; i++) {
        BOOST_CHECK_EQUAL(id_utils::generateMd5Hash(macToHash, id_utils::generateSalt(i)).length(), md5HashLength);
    }
}

BOOST_AUTO_TEST_CASE(idRange) {
    for (int i = 0; i < 1; i++) {
        uint32_t id = id_utils::generateId(macToHash, id_utils::generateSalt(i));
        BOOST_CHECK(id >= 1 && id <= UINT32_MAX );
    }
}

BOOST_AUTO_TEST_CASE(idType) {
    for (int i = 0; i < numberOfTestCases; i++) {
        BOOST_CHECK_EQUAL(typeid(id_utils::generateId(macToHash, id_utils::generateSalt(i))).name(), typeid(std::uint32_t).name());
    }
}

} // namespace elevation
