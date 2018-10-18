//
// Created by adam on 09/10/18.
//
#include "Database.hpp"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <stdexcept>

Database* Database::s_instance = nullptr;

Database* Database::instance() {

    if(!s_instance) {
        s_instance = new Database;
    }
    return s_instance;
}

/*
 * Returns an empty user (all 0s) on unsuccesful search
 */
void Database::getUserByMac(const char* mac,
                            struct User* __restrict__ user) const {
    int errcode = 0;
    const char* query = sqlite3_mprintf(
            "SELECT rowid, ip, mac, name FROM user WHERE (mac = '%q');", mac);

    sqlite3_stmt *statement = 0;
    errcode = sqlite3_prepare_v2(m_db, query, -1, &statement, 0); // TODO give exact strlen for perfo
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_step(statement);
    if (errcode == SQLITE_ROW) {
        user->id = sqlite3_column_int(statement, 0);
        strcpy(user->ip, (char *)sqlite3_column_text(statement, 1));
        strcpy(user->name, (char *)sqlite3_column_text(statement, 3));
        strcpy(user->mac, (char *)sqlite3_column_text(statement, 2)); // do last as a coherence check
    }
    return;
}

int Database::createUser(const struct User* user) {
    int errcode = 0;
    char* errmsg = NULL;
    const char* query = sqlite3_mprintf(
                "INSERT OR REPLACE INTO user VALUES ('%q', '%q', '%q');",
                user->ip,
                user->mac,
                user->name);

    sqlite3_stmt *res = 0;
    errcode = sqlite3_exec(m_db, query, NULL, NULL, &errmsg);
    if (errcode != SQLITE_OK) {
        std::string message;
        if (errmsg) {
            message = errmsg;
            free(errmsg);
        } else {
            message = "unknown database error";
        }
        throw std::runtime_error(message);
    }
//    errcode = sqlite3_prepare_v2(m_db, query, -1, &res, 0); // TODO give exact strlen for perfo
//    if (errcode)
//        goto err;
//
//    errcode = sqlite3_step(res);
//    if (errcode == SQLITE_ROW) {
//        user->id = sqlite3_column_int(res, 0);
//        strcpy(user->ip, (char *)sqlite3_column_text(res, 1));
//        strcpy(user->name, (char *)sqlite3_column_text(res, 3));
//        strcpy(user->mac, (char *)sqlite3_column_text(res, 2)); // do last as a coherence check
//        errcode = 0;
//    }
    return errcode;
}

Database::Database() {
    int errcode = sqlite3_open("server.db", &m_db);
    if (errcode) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        throw "Cannot connect to database";
    }
}
