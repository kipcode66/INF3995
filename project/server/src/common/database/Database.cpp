#include "Database.hpp"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <sstream>
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

void Database::assertSqliteOk(int errcode, const std::string& message) {
    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        std::stringstream what;
        what << message << "; Sqlite error message: " << sqlite3_errstr(errcode);
        throw sqlite_error(errcode, what.str());
    }
}

void Database::assertSqliteOk(int errcode) {
    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw sqlite_error(errcode);
    }
}

/*
 * Returns an empty user (all 0s) on unsuccessful search
 */
User_t Database::getUserByMac(const char* mac) const {
    User_t user = {0};
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT ip, mac, name, user_id FROM user WHERE (mac = '%q');", mac);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        strcpy(user.ip, (char *)sqlite3_column_text(statement, 0));
        strcpy(user.mac, (char *)sqlite3_column_text(statement, 1)); // do last as a coherence check
        strcpy(user.name, (char *)sqlite3_column_text(statement, 2));
        user.userId = sqlite3_column_int(statement, 3);
    }
    errcode = sqlite3_finalize(statement);
    assertSqliteOk(errcode);

    return std::move(user);
}

User_t Database::getUserById(uint32_t id) const {
    User_t user = {0};
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT user_id, ip, mac, name FROM user WHERE (user_id = %u);", id);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        user.userId = sqlite3_column_int(statement, 0);
        strcpy(user.ip, (char *)sqlite3_column_text(statement, 1));
        strcpy(user.name, (char *)sqlite3_column_text(statement, 3));
        strcpy(user.mac, (char *)sqlite3_column_text(statement, 2)); // do last as a coherence check
    }
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
    return user;
}

void Database::createUser(const User_t* user) {
    int errcode = 0;
    char* query = sqlite3_mprintf(
                "INSERT OR REPLACE INTO user (user_id, ip, mac, name) VALUES (%u, '%q', '%q', '%q');",
                user->userId,
                user->ip,
                user->mac,
                user->name);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
}

void Database::connectUser(const struct User_t* user) {
    uint32_t userIsConnected = 1;
    int errcode = 0;
    char* query = sqlite3_mprintf(
                "INSERT INTO userConnection VALUES (%u, %u, julianday('now'));",
                user->userId,
                userIsConnected);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
}

int Database::getUserConnectionStatus(uint32_t userId) const {
    int errcode = 0;
    int status = 0;
    char* query = sqlite3_mprintf(
            "SELECT isConnected FROM userConnection WHERE (user_id = %u);", userId);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        status = sqlite3_column_int(statement, 0);
    }
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
    return status;
}

void Database::createAdmin(const char* password) {
    int errcode = 0;
    std::string salt = id_utils::generateSalt(strlen(password));
    std::string passwordStr(password);
    std::string passwordHash = id_utils::generateMd5Hash(passwordStr, salt);
    char* query = sqlite3_mprintf(
                "INSERT INTO adminLogin VALUES ('admin', '%q', '%q');",
                passwordHash.c_str(),
                salt.c_str());

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
}

void Database::connectAdmin(const char* login, uint32_t adminId) {
    uint32_t adminIsConnected = 1;
    int errcode = 0;
    char* query = sqlite3_mprintf(
                "INSERT or REPLACE INTO adminConnection VALUES ('%q', %u, %u, julianday('now'));",
                login,
                adminIsConnected,
                adminId);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
}

void Database::disconnectAdmin(uint32_t adminId) {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "UPDATE adminConnection SET timeStamp = 0, isConnected = 0 WHERE (login = 'admin' AND admin_id = %u)", adminId);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0);
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
}

bool Database::isAdminConnected(uint32_t adminId) const {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT isConnected FROM adminConnection WHERE (login = 'admin' and admin_id = %u);", adminId);
    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    bool isConnected = false;
    if (errcode == SQLITE_ROW) {
        isConnected = (bool)sqlite3_column_int(statement, 0);
    }
    errcode = sqlite3_finalize(statement);

    assertSqliteOk(errcode);
    return isConnected;
}

std::pair<std::string, std::string> Database::getSaltAndHashedPasswordByLogin(const char* login) const {
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT salt, hashed_password FROM adminLogin WHERE (login = '%q');", login);
    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    std::pair<std::string, std::string> saltAndHashedPassword;
    if (errcode == SQLITE_ROW) {
        std::string salt((char *)sqlite3_column_text(statement, 0));
        std::string hash((char *)sqlite3_column_text(statement, 1));
        saltAndHashedPassword = std::make_pair(salt, hash);
    }
    errcode = sqlite3_finalize(statement);
    assertSqliteOk(errcode);

    return saltAndHashedPassword;
}

Song_t Database::getSongByQuery_(const char* query) const {
    Song_t song = {};
    int errcode = 0;

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo

    errcode = sqlite3_blocking_step(statement);
    if (errcode == SQLITE_ROW) {
        song.id = sqlite3_column_int(statement, 0);
        strcpy(song.title, (char *)sqlite3_column_text(statement, 1));
        strcpy(song.artist, (char *)sqlite3_column_text(statement, 2));
        song.userId = sqlite3_column_int(statement, 3);
        song.duration = sqlite3_column_int(statement, 4);
        strcpy(song.path, (char *)sqlite3_column_text(statement, 5));
    }
    errcode = sqlite3_finalize(statement);
    assertSqliteOk(errcode);

    return song;
}

Song_t Database::getSongById(int id) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, duration, path FROM cachedSong WHERE (rowid = %u);", id);
    Song_t song = getSongByQuery_(query);
    sqlite3_free(query);
    return song;
}

Song_t Database::getSongByTitle(const char* title) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, duration, path FROM cachedSong WHERE (title = '%q');", title);
    Song_t song = getSongByQuery_(query);
    sqlite3_free(query);
    return song;
}

Song_t Database::getSongByPath(const char* path) const {
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, duration, path FROM cachedSong WHERE (path = '%q');", path);
    Song_t song = getSongByQuery_(query);
    sqlite3_free(query);
    return song;
}

std::vector<Song_t> Database::getSongsByUser(int userId) const {
    std::vector<Song_t> songs;
    Song_t song_buffer;
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, duration, path FROM cachedSong WHERE (user_id = %u);", userId);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    while (errcode == SQLITE_ROW) {
        song_buffer.id = sqlite3_column_int(statement, 0);
        strcpy(song_buffer.title, (char *)sqlite3_column_text(statement, 1));
        strcpy(song_buffer.artist, (char *)sqlite3_column_text(statement, 2));
        song_buffer.userId = sqlite3_column_int(statement, 3);
        song_buffer.duration = sqlite3_column_int(statement, 4);
        strcpy(song_buffer.path, (char *)sqlite3_column_text(statement, 5));

        songs.push_back(song_buffer);
        errcode = sqlite3_blocking_step(statement);
    }
    errcode = sqlite3_finalize(statement);
    assertSqliteOk(errcode);

    return songs;
}

std::vector<Song_t> Database::getAllSongs() const {
    std::vector<Song_t> songs;
    Song_t song_buffer;
    int errcode = 0;
    char* query = sqlite3_mprintf(
            "SELECT rowid, title, artist, user_id, duration, path FROM cachedSong;");

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL); // strlen for perfo
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    while (errcode == SQLITE_ROW) {
        song_buffer.id = sqlite3_column_int(statement, 0);
        strcpy(song_buffer.title, (char *)sqlite3_column_text(statement, 1));
        strcpy(song_buffer.artist, (char *)sqlite3_column_text(statement, 2));
        song_buffer.userId = sqlite3_column_int(statement, 3);
        song_buffer.duration = sqlite3_column_int(statement, 4);
        strcpy(song_buffer.path, (char *)sqlite3_column_text(statement, 5));

        songs.push_back(song_buffer);
        errcode = sqlite3_blocking_step(statement);
    }
    errcode = sqlite3_finalize(statement);
    assertSqliteOk(errcode);

    return songs;
}

void Database::createSong(const Song_t* song) {
    int errcode = 0;
    char* query = sqlite3_mprintf(
                "INSERT OR REPLACE INTO cachedSong VALUES ('%q', '%q', %u, %u, '%q');",
                song->title,
                song->artist,
                song->userId,
                song->duration,
                song->path);

    sqlite3_stmt *statement = nullptr;
    sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
    sqlite3_free(query);

    errcode = sqlite3_blocking_step(statement);
    errcode = sqlite3_finalize(statement);
    assertSqliteOk(errcode);
}

void Database::enable_foreign_keys(sqlite3* m_db) {
    int errcode = 0;
    do {
        try {
            char* query = sqlite3_mprintf("PRAGMA foreign_keys = ON;");

            sqlite3_stmt *statement = nullptr;
            sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
            sqlite3_free(query);

            do {
                errcode = sqlite3_blocking_step(statement);
            } while (errcode == SQLITE_ROW);

            errcode = sqlite3_finalize(statement);
            assertSqliteOk(errcode);
            errcode = SQLITE_OK;
        }
        catch(sqlite_error& e) {
            std::this_thread::sleep_for(100ms);
        }
    } while(errcode == SQLITE_LOCKED);

    assertSqliteOk(errcode, "Cannot enable foreign keys");
}

void Database::wipeDbSongs(sqlite3* m_db) {
    int errcode = 0;
    do {
        try {
            char* query = sqlite3_mprintf("DELETE FROM cachedSong;");

            sqlite3_stmt *statement = nullptr;
            sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, NULL);
            sqlite3_free(query);

            do {
                errcode = sqlite3_blocking_step(statement);
            } while (errcode == SQLITE_ROW);

            errcode = sqlite3_finalize(statement);
            assertSqliteOk(errcode);
            errcode = SQLITE_OK;
        }
        catch(sqlite_error& e) {
            std::this_thread::sleep_for(100ms);
        }
    } while(errcode == SQLITE_LOCKED);

    assertSqliteOk(errcode, "Cannot wipe cached songs");
}

void Database::initDefaultAdmin(sqlite3* m_db) {
    int errcode = 0;
    do {
        try {
            char* query = sqlite3_mprintf(
                    "SELECT * FROM adminLogin;");
            sqlite3_stmt *statement = nullptr;
            sqlite3_blocking_prepare_v2(m_db, query, strlen(query), &statement, 0); // strlen for perfo
            sqlite3_free(query);

            errcode = sqlite3_blocking_step(statement);
            if (errcode == SQLITE_DONE) {
                createAdmin(DEFAULT_PASSWORD);
            }

            errcode = sqlite3_finalize(statement);
            assertSqliteOk(errcode);
        }
        catch (sqlite_error& e) {
            std::this_thread::sleep_for(100ms);
        }
    } while (errcode == SQLITE_LOCKED);

    assertSqliteOk(errcode, "Cannot create a default admin");
}

Database::Database() {
    assertSqliteOk(sqlite3_enable_shared_cache(true), "Cannot enable db shared cache mode");
    try {
        assertSqliteOk(
            sqlite3_open_v2(Database::DB_NAME, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, NULL),
            "Cannot connect to database");
        enable_foreign_keys(m_db);
        wipeDbSongs(m_db);
        initDefaultAdmin(m_db);
    } catch (...) {
        sqlite3_close(m_db);
        throw;
    }
}

Database::Database(std::experimental::filesystem::path serverPath) {
    assertSqliteOk(sqlite3_enable_shared_cache(true), "Cannot enable db shared cache mode");
    try {
        assertSqliteOk(
            sqlite3_open_v2(serverPath.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, NULL),
            "Cannot connect to database");
        enable_foreign_keys(m_db);
        wipeDbSongs(m_db);
        //initDefaultAdmin(m_db);
    } catch (...) {
        sqlite3_close(m_db);
        throw;
    }
}
