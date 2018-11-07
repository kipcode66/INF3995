#include "Database.hpp"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>

#include "misc/id_utils.hpp"
#include "thread_safe_sqlite.hpp"

using namespace elevation;
using namespace std::chrono_literals;

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
            "SELECT ip, mac, name, user_id FROM user WHERE (mac = '%q');", mac);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        strcpy(user->ip, (char *)sqlite3_column_text(statement, 0));
        strcpy(user->mac, (char *)sqlite3_column_text(statement, 1)); // do last as a coherence check
        strcpy(user->name, (char *)sqlite3_column_text(statement, 2));
        user->user_id = sqlite3_column_int(statement, 3);
    } else {
        *user = { 0 };
    }
    sqlite3_finalize(statement);
    return;
}

void Database::getUserById(int id, User_t* __restrict__ user) const {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT user_id, ip, mac, name FROM user WHERE (user_id = %u);", id);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        user->user_id = sqlite3_column_int(statement, 0);
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
    char* query = sqlite3_mprintf(
                "INSERT OR REPLACE INTO user (user_id, ip, mac, name) VALUES (%u, '%q', '%q', '%q');",
                user->user_id,
                user->ip,
                user->mac,
                user->name);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);

    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);
    errcode = SQLITE_OK;

    return errcode;
}

int Database::connectUser(const struct User_t* user) {
    uint32_t userIsConnected = 1;
    int errcode = 0;
    char* query = sqlite3_mprintf(
                "INSERT INTO userConnection VALUES (%u, %u, julianday('now'));",
                user->user_id,
                userIsConnected);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);

    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);
    errcode = SQLITE_OK;

    return errcode;
}

int Database::createAdmin(const char* password) {
    int errcode = 0;
    std::string salt = id_utils::generateSalt(strlen(password));
    std::string passwordStr(password);
    std::string passwordHash = id_utils::generateMd5Hash(passwordStr, salt);
    char* query = sqlite3_mprintf(
                "INSERT INTO adminLogin VALUES ('admin', '%q', '%q');",
                passwordHash.c_str(),
                salt.c_str());

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);

    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);
    errcode = SQLITE_OK;

    return errcode;
}

int Database::connectAdmin(const char* login, uint32_t admin_id) {
    uint32_t adminIsConnected = 1;
    int errcode = 0;
    char* query = sqlite3_mprintf(
                "INSERT or REPLACE INTO adminConnection VALUES ('%q', %u, %u, julianday('now'));",
                login,
                adminIsConnected,
                admin_id);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);

    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);
    errcode = SQLITE_OK;

    return errcode;
}

int Database::disconnectAdmin(uint32_t admin_id) {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "UPDATE adminConnection SET timeStamp = 0, isConnected = 0 WHERE (login = 'admin' AND admin_id = %u)", admin_id);

    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);

    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);
    errcode = SQLITE_OK;

    return errcode;
}

bool Database::isAdminConnected(uint32_t admin_id) const {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT isConnected FROM adminConnection WHERE (login = 'admin' and admin_id = %u);", admin_id);
    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    bool isConnected = false;
    if (errcode == SQLITE_ROW) {
        isConnected = (bool)sqlite3_column_int(statement, 0);
    }
    sqlite3_finalize(statement);
    return isConnected;
}

std::vector<char*> Database::getSaltAndHashedPasswordByLogin(const char* login) const {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT salt, hashed_password FROM adminLogin WHERE (login = '%q');", login);
    sqlite3_stmt *statement = nullptr;
    errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    if (errcode)
        throw std::runtime_error(sqlite3_errstr(errcode));

    errcode = sqlite3_blocking_step(statement);
    std::vector<char*> saltAndHashedPassword;
    if (errcode == SQLITE_ROW) {
        saltAndHashedPassword.push_back((char *)sqlite3_column_text(statement, 0));
        saltAndHashedPassword.push_back((char *)sqlite3_column_text(statement, 1));
    } else {
        errcode = sqlite3_finalize(statement);
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);
    return saltAndHashedPassword;
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
        sqlite3_finalize(statement);    
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    errcode = sqlite3_finalize(statement);
    return;
}

void Database::getSongById(int id, Song_t* __restrict__ song) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM cachedSong WHERE (rowid = %u);", id);
    getSongByQuery_(query, song);
    sqlite3_free(query);
}

void Database::getSongByTitle(const char* title, Song_t* __restrict__ song) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM cachedSong WHERE (title = '%q');", title);
    getSongByQuery_(query, song);
    sqlite3_free(query);
}

void Database::getSongByPath(const char* path, Song_t* __restrict__ song) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM cachedSong WHERE (path = '%q');", path);
    getSongByQuery_(query, song);
    sqlite3_free(query);
}

std::vector<Song_t> Database::getSongsByUser(int userId) const {
    std::vector<Song_t> songs;
    Song_t song_buffer;
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, path FROM cachedSong WHERE (user_id = %u);", userId);

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
            "SELECT rowid, title, artist, user_id, path FROM cachedSong;");

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
                "INSERT OR REPLACE INTO cachedSong VALUES ('%q', '%q', %i, '%q');",
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

    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw std::runtime_error(sqlite3_errstr(errcode));
    }
    sqlite3_finalize(statement);
    errcode = SQLITE_OK;

    return errcode;
}

int enable_foreign_keys(sqlite3* m_db, char **errmsg) {
    int errcode = 0;
    do {
        try {
            char* query = sqlite3_mprintf("PRAGMA foreign_keys = ON;");

            sqlite3_stmt *statement = nullptr;
            errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
            sqlite3_free(query);
            if (errcode)
                throw std::runtime_error(sqlite3_errstr(errcode));

            do {
                errcode = sqlite3_blocking_step(statement);
            } while (errcode == SQLITE_ROW);

            if (errcode != SQLITE_DONE
                && errcode != SQLITE_OK) {
                throw std::runtime_error(sqlite3_errstr(errcode));
            }
            sqlite3_finalize(statement);
            errcode = SQLITE_OK;
        }
        catch(std::runtime_error& e) {
            std::this_thread::sleep_for(100ms);
        }
    } while(errcode == SQLITE_LOCKED);

    return errcode;
}

int wipeDbSongs(sqlite3* m_db, char **errmsg) {
    int errcode = 0;
    do {
        try {
            char* query = sqlite3_mprintf("DELETE FROM cachedSong;");

            sqlite3_stmt *statement = nullptr;
            errcode = sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
            sqlite3_free(query);
            if (errcode)
                throw std::runtime_error(sqlite3_errstr(errcode));

            do {
                errcode = sqlite3_blocking_step(statement);
            } while (errcode == SQLITE_ROW);

            if (errcode != SQLITE_DONE
                && errcode != SQLITE_OK) {
                throw std::runtime_error(sqlite3_errstr(errcode));
            }
            sqlite3_finalize(statement);
            errcode = SQLITE_OK;
        }
        catch(std::runtime_error& e) {
            std::this_thread::sleep_for(100ms);
        }
    } while(errcode == SQLITE_LOCKED);

    return errcode;
}

Database::Database() {
    int errcode = 0;
    errcode = sqlite3_enable_shared_cache(true);
    if (errcode != SQLITE_OK) {
        std::cerr << "Can't enable db shared cache mode: " << sqlite3_errmsg(m_db) << std::endl;

        sqlite3_close(m_db);
        throw std::runtime_error("Cannot enable db shared cache mode");
    }
    errcode = sqlite3_open_v2(Database::DB_NAME, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, NULL);
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

