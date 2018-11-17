#define BOOST_TEST_MODULE database_test_framework

#include <boost/test/unit_test.hpp>

#include <database/sqlite_error.hpp>
#include <sqlite3/sqlite3.h>
#include <TestResources.hpp>
#include <database/Database.hpp>
#include <misc/id_utils.hpp>

namespace elevation {

class DatabaseTest : public Database {
public:
    static DatabaseTest* instance();

protected:
    DatabaseTest();
    sqlite3* m_db = 0;
    static DatabaseTest* s_instance;

};

DatabaseTest* DatabaseTest::s_instance = nullptr;

DatabaseTest::DatabaseTest() : Database(DATABASE_TEST_PATH) { }

DatabaseTest* DatabaseTest::instance() {
    if(!s_instance) {
        s_instance = new DatabaseTest;
    }
    return s_instance;
}

User_t createdUser = {
    12345678,
    "192.168.0.2",
    "11:22:33:44:55:77",
    "othmane"
};

BOOST_AUTO_TEST_CASE(getUserByMac) {
    User_t user = { 0 };
    DatabaseTest* db = DatabaseTest::instance();
    user = db->getUserByMac("11:22:33:44:55:66");
    BOOST_CHECK_EQUAL(user.userId, 123456789);
    BOOST_CHECK_EQUAL(user.name, "othman");
    BOOST_CHECK_EQUAL(user.ip, "192.168.0.1");
}

BOOST_AUTO_TEST_CASE(getUserById) {
    User_t user = { 0 };
    DatabaseTest* db = DatabaseTest::instance();
    user = db->getUserById(123456789);
    BOOST_CHECK_EQUAL(user.mac, "11:22:33:44:55:66");
    BOOST_CHECK_EQUAL(user.name, "othman");
    BOOST_CHECK_EQUAL(user.ip, "192.168.0.1");
}

BOOST_AUTO_TEST_CASE(createUser) {
    DatabaseTest* db = DatabaseTest::instance();
    db->createUser(&createdUser);

    User_t user = db->getUserById(12345678);
    BOOST_CHECK_EQUAL(user.mac, createdUser.mac);
    BOOST_CHECK_EQUAL(user.userId, createdUser.userId);
    BOOST_CHECK_EQUAL(user.name, createdUser.name);
    BOOST_CHECK_EQUAL(user.ip, createdUser.ip);
}

BOOST_AUTO_TEST_CASE(setAdminPassword) {
    DatabaseTest* db = DatabaseTest::instance();
    db->setAdminPassword("admin");

    std::pair<std::string, std::string> saltAndHash =  db->getSaltAndHashedPasswordByLogin("admin");
    std::string hashedPassword = id_utils::generateMd5Hash("admin", std::get<0>(saltAndHash));

    BOOST_CHECK_EQUAL(hashedPassword, std::get<1>(saltAndHash));
}

BOOST_AUTO_TEST_CASE(connectUser) {
    DatabaseTest* db = DatabaseTest::instance();
    db->connectUser(&createdUser);

    int connectionStatus = db->getUserConnectionStatus(createdUser.userId);

    BOOST_CHECK_EQUAL(connectionStatus, 1);
}

BOOST_AUTO_TEST_CASE(connectAdmin) {
    DatabaseTest* db = DatabaseTest::instance();
    db->connectAdmin("admin", 123456789);

    bool connectionStatus = db->isAdminConnected(123456789);

    BOOST_CHECK_EQUAL(connectionStatus, true);
}

BOOST_AUTO_TEST_CASE(disconnectAdmin) {
    DatabaseTest* db = DatabaseTest::instance();
    db->disconnectAdmin(123456789);

    bool connectionStatus = db->isAdminConnected(123456789);

    BOOST_CHECK_EQUAL(connectionStatus, false);
}

} // namespace elevation
