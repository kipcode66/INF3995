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
#include "Query.hpp"
#include "templates/exception/NoSuchUserException.hpp"

using namespace elevation;
using namespace std::chrono_literals;


Database* Database::s_instance = nullptr;
constexpr const char Database::DB_NAME[] = "server.db";

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
        if (message.length() > 0) {
            std::ostringstream what;
            what << message << "; Sqlite error message: " << sqlite3_errstr(errcode);
            throw sqlite_error(errcode, what.str());
        }
        else {
            throw sqlite_error(errcode);
        }
    }
}

void Database::executeQuery_(const Query& query) {
    Statement stmt{m_db, query};
    stmt.step();
}

User_t Database::getUserFromStatement_(const Statement& stmt) const {
    User_t user = { 0 };
    user.userId = stmt.getColumnInt(0);
    strncpy(user.ip, stmt.getColumnText(1).c_str(), User_t::IP_LENGTH);
    strncpy(user.name, stmt.getColumnText(2).c_str(), User_t::NAME_LENGTH);
    strncpy(user.mac, stmt.getColumnText(3).c_str(), User_t::MAC_LENGTH);
    user.isBlocked = stmt.getColumnInt(4);
    return user;
}

Song_t Database::getSongFromStatement_(const Statement& stmt) const {
    Song_t song = { 0 };
        song.id = stmt.getColumnInt(0);
        strncpy(song.title, stmt.getColumnText(1).c_str(), Song_t::TITLE_LENGTH);
        strncpy(song.artist, stmt.getColumnText(2).c_str(), Song_t::TITLE_LENGTH);
        song.userId = stmt.getColumnInt(3);
        song.duration = stmt.getColumnInt(4);
        strncpy(song.path, stmt.getColumnText(5).c_str(), Song_t::PATH_LENGTH);
        song.deletedByAdmin = stmt.getColumnBool(6);
    return song;
}

User_t Database::getUserByQuery_(const Query& query) const {
    User_t user = { 0 };
    Statement stmt{m_db, query};
    if (stmt.step()) {
        user = getUserFromStatement_(stmt);
    }
    return user;
}

std::vector<User_t> Database::getUsersByQuery_(const Query& query) const {
    std::vector<User_t> users;
    Statement stmt{m_db, query};
    while (stmt.step()) {
        users.push_back(getUserFromStatement_(stmt));
    }
    return users;
}

/*
 * Returns an empty user (all 0s) on unsuccessful search
 */
User_t Database::getUserByMac(const std::string& mac) const {
    return getUserByQuery_(Query(
        "SELECT user_id, ip, name, mac FROM user WHERE (mac = '%q');",
        mac.c_str()));
}

User_t Database::getUserById(uint32_t id) const {
    return getUserByQuery_(Query(
        "SELECT user_id, ip, name, is_blacklisted, mac FROM user WHERE (user_id = %u);",
        id));
}

void Database::createUser(const User_t* user) {
    executeQuery_(Query(
        "INSERT OR REPLACE INTO user (user_id, ip, mac, name) "
        "VALUES (%u, '%q', '%q', '%q');",
        user->userId,
        user->ip,
        user->mac,
        user->name));
}

void Database::connectUser(const struct User_t* user) {
    executeQuery_(Query(
        "INSERT INTO userConnection (user_id, connection_expiration) VALUES (%u, julianday('now', '+1 day'));",
        user->userId));
}

bool Database::isUserConnected(const uint32_t userId) const {
    Statement stmt{m_db, Query(
        "SELECT julianday(connection_expiration) - julianday('now') FROM userConnection "
        "WHERE (user_id = %u);",
        userId)};

    if (stmt.step()) {
        return (stmt.getColumnDouble(0) > 0);
    }
    return false;
}

void Database::setAdminPassword(const std::string& password) {
    std::string salt = id_utils::generateSalt(password.length());
    std::string passwordHash = id_utils::generateMd5Hash(password, salt);
    executeQuery_(Query(
        "INSERT OR REPLACE INTO adminLogin VALUES ('admin', '%q', '%q');",
        passwordHash.c_str(),
        salt.c_str()));
}

void Database::connectAdmin(const std::string& login, uint32_t adminId) {
    uint32_t adminIsConnected = 1;
    executeQuery_(Query(
        "INSERT or REPLACE INTO adminConnection "
        "VALUES ('%q', %u, %u, julianday('now'));",
        login.c_str(),
        adminIsConnected,
        adminId));
}

void Database::disconnectAdmin(uint32_t adminId) {
    executeQuery_(Query(
        "UPDATE adminConnection SET timeStamp = 0, isConnected = 0 "
        "WHERE (login = 'admin' AND admin_id = %u)",
        adminId));
}

bool Database::isAdminConnected(uint32_t adminId) const {
    Statement stmt{m_db, Query(
        "SELECT isConnected FROM adminConnection "
        "WHERE (login = 'admin' and admin_id = %u);",
        adminId)};

    if (stmt.step()) {
        return stmt.getColumnBool(0);
    }
    return false;
}

std::pair<std::string, std::string> Database::getSaltAndHashedPasswordByLogin(
        const std::string& login) const {
    Statement stmt{m_db, Query(
        "SELECT salt, hashed_password FROM adminLogin WHERE (login = '%q');",
        login.c_str())};

    std::pair<std::string, std::string> saltAndHashedPassword;
    if (stmt.step()) {
        std::string salt = stmt.getColumnText(0);
        std::string hash = stmt.getColumnText(1);
        saltAndHashedPassword = std::make_pair(salt, hash);
    }

    return saltAndHashedPassword;
}

std::vector<User_t> Database::getBlackList() {
    return getUsersByQuery_(Query(
        "SELECT user_id, ip, name, mac FROM user"
        " WHERE (is_blacklisted = %i);", Database::BLOCKED));
}

Song_t Database::getSongByQuery_(const Query& query) const {
    Song_t song = { 0 };
    Statement stmt{m_db, query};

    if (stmt.step()) {
        song = getSongFromStatement_(stmt);
    }

    return song;
}

Song_t Database::getSongById(int id) const {
    return getSongByQuery_(Query(
        "SELECT rowid, title, artist, user_id, duration, path FROM songs "
        "WHERE (rowid = %u);",
        id));
}

Song_t Database::getSongByTitle(const std::string& title) const {
    return getSongByQuery_(Query(
        "SELECT rowid, title, artist, user_id, duration, path FROM songs "
        "WHERE (title = '%q');",
        title.c_str()));
}

Song_t Database::getSongByPath(const std::string& path) const {
    return getSongByQuery_(Query(
        "SELECT rowid, title, artist, user_id, duration, path FROM songs "
        "WHERE (path = '%q');",
        path.c_str()));
}

std::vector<Song_t> Database::getSongsByQuery_(const Query& query) const {
    std::vector<Song_t> songs;
    Song_t song_buffer;
    Statement stmt{m_db, query};

    while (stmt.step()) {
        song_buffer = getSongFromStatement_(stmt);
        songs.push_back(song_buffer);
    }

    return songs;
}

std::vector<Song_t> Database::getSongsByUser(int userId) const {
    return getSongsByQuery_(Query(
        "SELECT rowid, title, artist, user_id, duration, path, song_order "
        "FROM songs WHERE (user_id = %u AND was_played = 0 AND length(path) > 0) ORDER BY song_order ASC;",
        userId));
}

std::vector<Song_t> Database::getAllSongs() const {
    return getSongsByQuery_(Query(
        "SELECT rowid, title, artist, user_id, duration, path, song_order "
        "FROM songs ORDER BY song_order ASC;"));
}

std::vector<Song_t> Database::getAllPlayableSongs() const {
    return getSongsByQuery_(Query(
        "SELECT rowid, title, artist, user_id, duration, path, song_order "
        "FROM songs WHERE was_played = 0 AND length(path) > 0 "
        "ORDER BY song_order ASC;"));
}

int Database::getStatisticsFromQuery_(const Query& query) const {
    int statistic;
    Statement stmt{m_db, query};

    if (stmt.step()) {
        statistic = stmt.getColumnInt(0);
    }
    return statistic;
}

int Database::getDailyUserCount_() const {
    std::string query("SELECT COUNT(DISTINCT user_id) FROM songs WHERE");
    query += TODAY_QUERY;
    return getStatisticsFromQuery_(query.c_str());
}

int Database::getDailySongCount_() const {
    std::string query("SELECT COUNT(*) FROM songs WHERE was_played = 1 AND");
    query += TODAY_QUERY;
    return getStatisticsFromQuery_(query.c_str());
}

int Database::getDeletedSongsCount_() const {
    std::string query("SELECT COUNT(*) FROM songs WHERE deleted_by_admin = 1 AND");
    query += TODAY_QUERY;
    return getStatisticsFromQuery_(query.c_str());
}

int Database::getAverageSongDuration_() const {
    std::string query("SELECT avg(duration) FROM songs WHERE was_played = 1 AND");
    query += TODAY_QUERY;
    return getStatisticsFromQuery_(query.c_str());
}

Statistics Database::getStatistics() const {
    return Statistics(getDailySongCount_(), getDailyUserCount_(),
        getDeletedSongsCount_(), getAverageSongDuration_());
}

void Database::createSong(const Song_t* song) {
    executeQuery_(Query(
        "INSERT OR REPLACE INTO songs (title, artist, user_id, duration, path, song_order, timestamp, was_played) VALUES ('%q', '%q', %u, %u, '%q', %i, julianday('now'), %u);",
        song->title,
        song->artist,
        song->userId,
        song->duration,
        song->path,
        DEFAULT_SONG_ORDER,
        0));
}

/*
 * A null path indicate a deleted song
 */
void Database::removeSong(uint32_t id, bool wasPlayed) {
    executeQuery_(Query(
        "UPDATE songs SET was_played = %i, path = '' WHERE rowid = %i;",
        wasPlayed,
        id));
}

/*
 * A null path indicate a deleted song
 */
void Database::removeSongByAdmin(uint32_t id, bool wasPlayed) {
    executeQuery_(Query(
        "UPDATE songs SET was_played = %i, path = '', "
        "deleted_by_admin = 1 WHERE rowid = %i;",
        wasPlayed,
        id));
}

void Database::executeAndRetryOnLock_(const Query& query) {
    bool retry = false;
    do {
        retry = false;
        try {
            Statement stmt(m_db, query);
            while(stmt.step());
        }
        catch(sqlite_error& e) {
            if (e.code() == SQLITE_LOCKED ||
                e.code() == SQLITE_BUSY) {
                std::this_thread::sleep_for(100ms);
                retry = true;
            }
            else {
                throw e;
            }
        }
    } while (retry);
}

void Database::enableForeignKeys_() {
    executeAndRetryOnLock_(Query("PRAGMA foreign_keys = ON;"));
}

void Database::wipeDbSongs_() {
    executeAndRetryOnLock_(Query("UPDATE songs SET path = '';"));
}

void Database::initDefaultAdmin() {
    bool retry = false;
    do {
        retry = false;
        try {
            Statement stmt(m_db, Query{"SELECT * FROM adminLogin;"});
            if (!stmt.step()) {
                setAdminPassword(DEFAULT_PASSWORD);
            }
        }
        catch(sqlite_error& e) {
            if (e.code() == SQLITE_LOCKED ||
                e.code() == SQLITE_BUSY) {
                std::this_thread::sleep_for(100ms);
                retry = true;
            }
            else {
                throw e;
            }
        }
    } while(retry);
}

Database::Database() {
    assertSqliteOk(!sqlite3_threadsafe());
    assertSqliteOk(sqlite3_initialize(), "Unable to initialize SQLite");
    assertSqliteOk(sqlite3_enable_shared_cache(true), "Cannot enable db shared cache mode");
    try {
        assertSqliteOk(
            sqlite3_open_v2(Database::DB_NAME, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, NULL),
            "Cannot connect to database");
        enableForeignKeys_();
        wipeDbSongs_();
        initDefaultAdmin();
    } catch (...) {
        sqlite3_close_v2(m_db);
        throw;
    }
}

Database::Database(std::experimental::filesystem::path serverPath) {
    assertSqliteOk(sqlite3_enable_shared_cache(true), "Cannot enable db shared cache mode");
    try {
        assertSqliteOk(
            sqlite3_open_v2(serverPath.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, NULL),
            "Cannot connect to database");
        sqlite3_busy_handler(m_db, [](void* pArg, int n) {return 1;}, nullptr);
        enableForeignKeys_();
        wipeDbSongs_();
    } catch (...) {
        sqlite3_close_v2(m_db);
        throw;
    }
}


bool Database::getBlacklistByMAC(const std::string& mac) const {
    Statement stmt{m_db, Query(
        "SELECT is_blacklisted FROM user "
        "WHERE mac = '%q';",
        mac.c_str())};
    if(stmt.step()) {
        return (stmt.getColumnInt(0) == Database::BLOCKED);
    } else {
        throw NoSuchUserException();
    }
}

void Database::setBlacklistFlag_(const std::string& mac, bool flag) {
    executeQuery_(Query(
        "UPDATE user "
        "SET is_blacklisted = %i "
        "WHERE (mac = '%q');",
        flag,
        mac.c_str()));
}

void Database::blacklistMAC(const std::string& mac) {
    setBlacklistFlag_(mac, Database::BLOCKED);
}

void Database::whitelistMAC(const std::string& mac) {
    setBlacklistFlag_(mac, Database::UNBLOCKED);
}

Database::~Database() {
    sqlite3_close_v2(m_db);
    sqlite3_shutdown();
}

