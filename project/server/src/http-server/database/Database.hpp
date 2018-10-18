#include <sqlite3/sqlite3.h>
#include <stdint.h>

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

namespace elevation {

struct User {
    int64_t id;
    char ip[16];
    char mac[18];
    char name[256];
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
