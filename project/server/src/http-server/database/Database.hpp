#include <sqlite3/sqlite3.h>
#include <stdint.h>
#include <cstddef>

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

namespace elevation {


struct User_t {
    static const size_t IP_LENGTH = 16;
    static const size_t MAC_LENGTH = 18;
    static const size_t NAME_LENGTH = 256;
    static const size_t ID_LENGTH = 64;
    static const size_t TOKEN_LENGTH = 32;

    char id[ID_LENGTH];
    char ip[IP_LENGTH];
    char mac[MAC_LENGTH];
    char name[NAME_LENGTH];
    char token[TOKEN_LENGTH];
};


class Database {
public:
    static Database* instance();
    void getUserByMac(const char* mac, User_t* __restrict__ user) const;
    int createUser(const User_t* user);
    int updateTimestamp(const User_t* user);
    int connectUser(const struct User_t* user);

private:
    Database();

    sqlite3* m_db = 0;
    static Database* s_instance;
    const static char DB_NAME[];
};

} // namespace elevation


#endif // DATABASE_DATABASE_HPP
