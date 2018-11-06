#include "Database.hpp"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdexcept>
#include <iostream>

#include "thread_safe_sqlite.hpp"

using namespace elevation;

Database* Database::s_instance = nullptr;
constexpr char Database::DB_NAME[] = "server.db";

Database* Database::instance() {

    if(!s_instance) {
        s_instance = new Database;
    }
    return s_instance;
}

/*
 * Returns an empty user (all 0s) on unsuccessful search
 */
void Database::getUserByMac(const char* mac,
                            User_t* __restrict__ user) const {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT rowid, ip, mac, name FROM user WHERE (mac = '%q');", mac);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        user->id = sqlite3_column_int(statement, 0);
        strcpy(user->ip, (char *)sqlite3_column_text(statement, 1));
        strcpy(user->name, (char *)sqlite3_column_text(statement, 3));
        strcpy(user->mac, (char *)sqlite3_column_text(statement, 2)); // do last as a coherence check
    } else {
        *user = { 0 };
    }
    sqlite3_finalize(statement);
    return;
}

void Database::getUserById(int id, User_t* __restrict__ user) const {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT rowid, ip, mac, name FROM user WHERE (rowid = %i);", id);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        user->id = sqlite3_column_int(statement, 0);
        strcpy(user->ip, (char *)sqlite3_column_text(statement, 1));
        strcpy(user->name, (char *)sqlite3_column_text(statement, 3));
        strcpy(user->mac, (char *)sqlite3_column_text(statement, 2)); // do last as a coherence check
    } else {
        *user = { 0 };
    }
    sqlite3_finalize(statement);
    return;
}

int Database::createUser(const User_t* user) {
    int errcode = 0;
    char* errmsg = nullptr;
    char* query = sqlite3_mprintf(
                "INSERT OR REPLACE INTO user (rowid, ip, mac, name) VALUES (%i, '%q', '%q', '%q');",
                user->id,
                user->ip,
                user->mac,
                user->name);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
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
    sqlite3_finalize(statement);

    return errcode;
}

void Database::getSongByQuery_(const char* query, Song_t* __restrict__ song) const {
    int errcode = 0;

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        song->id = sqlite3_column_int(statement, 0);
        strcpy(song->title, (char *)sqlite3_column_text(statement, 1));
        strcpy(song->artist, (char *)sqlite3_column_text(statement, 2));
        song->user_id = sqlite3_column_int(statement, 3);
        strcpy(song->path, (char *)sqlite3_column_text(statement, 4));
    } else {
        *song = { 0 };
    }
    errcode = sqlite3_finalize(statement);
    return;
}

void Database::getSongById(int id, Song_t* __restrict__ song) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM song WHERE (rowid = %i);", id);
    getSongByQuery_(query, song);
    sqlite3_free(query);
}

void Database::getSongByTitle(const char* title, Song_t* __restrict__ song) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM song WHERE (title = '%q');", title);
    getSongByQuery_(query, song);
    sqlite3_free(query);
}

void Database::getSongByPath(const char* path, Song_t* __restrict__ song) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM song WHERE (path = '%q');", path);
    getSongByQuery_(query, song);
    sqlite3_free(query);
}

std::vector<Song_t> Database::getSongsByUser(int userId) const {
    std::vector<Song_t> songs;
    Song_t song_buffer;
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM song WHERE (user_id = %i);", userId);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    while (errcode == SQLITE_ROW) {
        song_buffer.id = sqlite3_column_int(statement, 0);
        strcpy(song_buffer.title, (char *)sqlite3_column_text(statement, 1));
        strcpy(song_buffer.artist, (char *)sqlite3_column_text(statement, 2));
        song_buffer.user_id = sqlite3_column_int(statement, 3);
        strcpy(song_buffer.path, (char *)sqlite3_column_text(statement, 4));

        songs.push_back(song_buffer);
        errcode = sqlite3_blocking_step(statement);
    }

    if (errcode != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);

    return songs;
}

std::vector<Song_t> Database::getAllSongs() const {
    std::vector<Song_t> songs;
    Song_t song_buffer;
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM song;");

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL); // strlen for perfo
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    while (errcode == SQLITE_ROW) {
        song_buffer.id = sqlite3_column_int(statement, 0);
        strcpy(song_buffer.title, (char *)sqlite3_column_text(statement, 1));
        strcpy(song_buffer.artist, (char *)sqlite3_column_text(statement, 2));
        song_buffer.user_id = sqlite3_column_int(statement, 3);
        strcpy(song_buffer.path, (char *)sqlite3_column_text(statement, 4));

        songs.push_back(song_buffer);
        errcode = sqlite3_blocking_step(statement);
    }

    if (errcode != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);

    return songs;
}

int Database::createSong(const Song_t* song) {
    int errcode = 0;
    char* query = sqlite3_mprintf(
                "INSERT OR REPLACE INTO song VALUES ('%q', '%q', %i, '%q');",
                song->title,
                song->artist,
                song->user_id,
                song->path);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    do {
        errcode = sqlite3_blocking_step(statement);
    } while (errcode == SQLITE_ROW);

    if (errcode != SQLITE_DONE
        || errcode != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);

    return errcode;
}

int enable_foreign_keys(sqlite3* m_db, char **errmsg) {
    int errcode = 0;
    char* query = sqlite3_mprintf("PRAGMA foreign_keys = ON;");

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    do {
        errcode = sqlite3_blocking_step(statement);
    } while (errcode == SQLITE_ROW);

    if (errcode != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);

    return errcode;
}

int wipeDbSongs(sqlite3* m_db, char **errmsg) {
    int errcode = 0;
    char* query = sqlite3_mprintf("DELETE FROM song;");

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    do {
        errcode = sqlite3_blocking_step(statement);
    } while (errcode == SQLITE_ROW);

    if (errcode != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);

    return errcode;
}

Database::Database() {
    int errcode = sqlite3_open(Database::DB_NAME, &m_db);
    if (errcode) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(m_db) << std::endl;

        sqlite3_close(m_db);
        throw std::runtime_error("Cannot connect to database");
    }
    char* errmsg = nullptr;
    errcode = enable_foreign_keys(m_db, &errmsg);
    if (errcode) {
        std::cerr << "Can't enable foreign keys: " << errmsg << std::endl;

        sqlite3_close(m_db);
        throw std::runtime_error("Cannot enable foreign keys");
    }
    errcode = wipeDbSongs(m_db, &errmsg);
    if (errcode) {
        std::cerr << "Can't wipe all songs: " << errmsg << std::endl;

        sqlite3_close(m_db);
        throw std::runtime_error("Cannot wipe all songs");
    }
}

