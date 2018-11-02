#include <sqlite3/sqlite3.h>
#include <stdint.h>
#include <cstddef>
#include "User.hpp"
#include <string>

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

namespace elevation {

class Database {
public:
    static Database* instance();
    void getUserByMac(const char* mac, User_t* __restrict__ user) const;
    int createUser(const User_t* user) const;
    int updateTimestamp(const User_t* user) const;
    int connectUser(const struct User_t* user) const;
    //int connectAdmin(const std::string& login, const std::string& password) const;
    char* getSaltByLogin(const char* login) const;

private:
    Database();

    sqlite3* m_db = 0;
    static Database* s_instance;
    const static char DB_NAME[];
};

} // namespace elevation


#endif // DATABASE_DATABASE_HPP
