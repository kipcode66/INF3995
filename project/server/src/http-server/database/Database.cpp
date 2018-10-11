//
// Created by adam on 09/10/18.
//
#include "Database.hpp"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

Database* Database::s_instance = nullptr;

Database* Database::instance() {

    if(!s_instance) {
        s_instance = new Database;
    }
    return s_instance;
}

struct User Database::getUserByMac(const char* mac) const {
    struct User user = { 0 };
    uint8_t errcode = 0;
    const char* query = sqlite3_mprintf("SELECT * FROM user WHERE (mac = '%q');", mac);
    sqlite3_stmt *res = 0;
    errcode = sqlite3_prepare_v2(m_db, query, -1, &res, 0);
    if (errcode)
        goto err;
    errcode = sqlite3_step(res);

    if (errcode == SQLITE_ROW) {
        strcpy(user.ip, (char *)sqlite3_column_text(res, 0));
        strcpy(user.mac, (char *)sqlite3_column_text(res, 1));
        strcpy(user.name, (char *)sqlite3_column_text(res, 2));
        goto done;
    }
err:
    printf("error: %s\n", sqlite3_errstr(errcode));
done:
    return user;
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
