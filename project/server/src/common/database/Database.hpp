#include <sqlite3/sqlite3.h>
#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>

#include "User.hpp"
#include "Song.hpp"
#include "sqlite_error.hpp"

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

namespace elevation {

class Database {
public:
    static constexpr const char* DEFAULT_PASSWORD = "admin";    
public:
    static Database* instance();
    User_t getUserByMac(const char*) const;
    User_t getUserById(uint32_t) const;
    void createUser(const User_t* user);
    void createAdmin(const char* password);
    void updateTimestamp(const User_t* user);
    void connectUser(const struct User_t* user);
    void connectAdmin(const char* login, uint32_t adminId);
    void disconnectAdmin(uint32_t adminId);
    bool isAdminConnected(uint32_t adminId) const;
    std::pair<std::string, std::string> getSaltAndHashedPasswordByLogin(const char* login) const;
    
    Song_t getSongById(int) const;
    Song_t getSongByTitle(const char*) const;
    Song_t getSongByPath(const char*) const;
    std::vector<Song_t> getSongsByUser(int userId) const;
    std::vector<Song_t> getAllSongs() const;
    void createSong(const Song_t*);
    void removeSong(uint32_t);

    void initDefaultAdmin(sqlite3* m_db);

private:
    Database();
    Song_t getSongByQuery_(const char*) const;

    sqlite3* m_db = 0;
    static Database* s_instance;
    const static char DB_NAME[];
};

} // namespace elevation


#endif // DATABASE_DATABASE_HPP
