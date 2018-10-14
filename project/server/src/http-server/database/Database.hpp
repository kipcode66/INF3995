//
// Created by adam on 09/10/18.
//
#include <sqlite3/sqlite3.h>
#include <stdint.h>

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

struct User {
    int64_t id;
    char ip[16];
    char mac[18];
    char name[256];
};


class Database {
public:
    static Database* instance();
    int getUserByMac(const char*, struct User* __restrict__) const;
    int createUser(const struct User*);

private:
    Database();

    sqlite3* m_db = 0;
    static Database* s_instance;
};
#endif // DATABASE_DATABASE_HPP
