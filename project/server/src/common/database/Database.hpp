#include <sqlite3/sqlite3.h>
#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <experimental/filesystem>

#include "templates/User.hpp"
#include "templates/Song.hpp"
#include "sqlite_error.hpp"
#include "Query.hpp"

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

namespace elevation {

class Database {
public:
    static constexpr const char* DEFAULT_PASSWORD = "admin";
    static constexpr const int32_t DEFAULT_SONG_ORDER = 0;

    static Database* instance();
    static void assertSqliteOk(int errcode, const std::string& message = "");


protected:
    static Database* s_instance;
    static const char DB_NAME[];

public:
    User_t getUserByMac(const std::string&) const;
    User_t getUserById(uint32_t) const;
    void createUser(const User_t* user);
    void createAdmin(const std::string& password);
    void updateTimestamp(const User_t* user);
    void connectUser(const struct User_t* user);
    void connectAdmin(const std::string& login, uint32_t adminId);
    void disconnectAdmin(uint32_t adminId);
    bool isAdminConnected(uint32_t adminId) const;
    std::pair<std::string, std::string> getSaltAndHashedPasswordByLogin(const std::string& login) const;
    
    Song_t getSongById(int) const;
    Song_t getSongByTitle(const std::string&) const;
    Song_t getSongByPath(const std::string&) const;
    std::vector<Song_t> getSongsByUser(int userId) const;
    std::vector<Song_t> getAllSongs() const;
    void createSong(const Song_t*);
    void removeSong(uint32_t);

    void initDefaultAdmin();

    int getUserConnectionStatus(uint32_t userId) const;

protected:
    Database();
    Database(std::experimental::filesystem::path);

    void executeAndRetryOnLock_(const Query&);
    void enableForeignKeys_();
    void wipeDbSongs_();

    Song_t getSongByQuery_(const Query&) const;
    std::vector<Song_t> getSongsByQuery_(const Query&) const;
    User_t getUserByQuery_(const Query&) const;
    void executeQuery_(const Query& query);

    sqlite3* m_db = 0;
};

} // namespace elevation


#endif // DATABASE_DATABASE_HPP
