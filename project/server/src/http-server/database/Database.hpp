#include <sqlite3/sqlite3.h>
#include <stdint.h>
#include <cstddef>

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

namespace elevation {

struct User {
    static const size_t IP_LENGTH = 16;
    static const size_t MAC_LENGTH = 18;
    static const size_t NAME_LENGTH = 256;

    int64_t id;
    char ip[IP_LENGTH];
    char mac[MAC_LENGTH];
    char name[NAME_LENGTH];
};


class Database {
public:
    static Database* instance();
    void getUserByMac(const char*, struct User* __restrict__) const;
    int createUser(const struct User*);

private:
    Database();

    sqlite3* m_db = 0;
    static Database* s_instance;
};

} // namespace elevation


#endif // DATABASE_DATABASE_HPP
