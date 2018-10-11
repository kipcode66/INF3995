//
// Created by adam on 09/10/18.
//
#include <sqlite3/sqlite3.h>

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

struct User {
    char ip[15];
    char mac[17];
    char name[255];
};


class Database {
public:
    static Database* instance();
    struct User getUserByMac(const char*) const;
    int createUser(struct User*);

private:
    Database();

    sqlite3* m_db = 0;
    static Database* s_instance;
};
#endif // DATABASE_DATABASE_HPP
