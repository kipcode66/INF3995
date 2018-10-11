//
// Created by adam on 09/10/18.
//
#include "Database.hpp"
#include <stdio.h>

Database* Database::s_instance = nullptr;

Database* Database::instance() {

    if(!s_instance) {
        s_instance = new Database;
    }
    return s_instance;
}

struct User Database::getUserByMac(const char* mac) const {
    struct User user;
    const char* query = sqlite3_mprintf("select * from user where (mac = '%q');", mac);
//    int errcode = sqlite3_exec(m_db, query, )
}

int Database::createUser(struct User* user) {
    return 0;
}

Database::Database() {
    int errcode = sqlite3_open("server.db", &m_db);
    if (errcode) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        throw "Cannot connect to database";
    }
}

//    struct User mocked_user = {
//            .ip = "23.2.2.12",
//            .mac = "FE:FE:FE:FE:FE:FE",
//            .nom = "Jerry",
//    };
//
//    // Open db
//    sqlite3* db;
//    int rc = ::sqlite3_open("server.db", &db);
//
//
//    // search for user in db
//
//    const char* SQL_ADD_USER = "SELECT * FROM user WHERE (ip";
//    char* sqlErrMsg = 0;
//    rc = sqlite3_exec(db, SQL_ADD_USER, callback, NULL, &sqlErrMsg);
//    if (rc != SQLITE_OK) {
//        puts("that didn't go well");
//        printf("SQL error: %s\n", sqlErrMsg);
//        sqlite3_free(sqlErrMsg);
//    }
//    // create entry in user
//    const char* SQL_ADD_USER = "INSERT INTO user (ip, mac, name) VALUES ('122.22.22.22', 'EE:FE:EE:EE:EE:EE', 'moi');";
//    char* sqlErrMsg = 0;
//    rc = sqlite3_exec(db, SQL_ADD_USER, callback, NULL, &sqlErrMsg);
//    if (rc != SQLITE_OK) {
//        puts("that didn't go well");
//        printf("SQL error: %s\n", sqlErrMsg);
//        sqlite3_free(sqlErrMsg);
//    }
