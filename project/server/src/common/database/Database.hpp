#include <sqlite3/sqlite3.h>
#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>

#include "User.hpp"
#include "Song.hpp"

#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

namespace elevation {

class Database {
public:
    static Database* instance();
    void getUserByMac(const char* mac, User_t* __restrict__ user) const;
    void getUserById(int, User_t* __restrict__) const;
    int createUser(const User_t* user);
    int createAdmin(const char* password);
    int updateTimestamp(const User_t* user);
    int connectUser(const struct User_t* user);
    int connectAdmin(const char* login, uint32_t admin_id);
    int disconnectAdmin(uint32_t admin_id);
    bool isAdminConnected(uint32_t admin_id) const;
    std::vector<char*> getSaltAndHashedPasswordByLogin(const char* login) const;
    
    void getSongById(int, Song_t* __restrict__) const;
    void getSongByTitle(const char*, Song_t* __restrict__) const;
    void getSongByPath(const char*, Song_t* __restrict__) const;
    std::vector<Song_t> getSongsByUser(int userId) const;
    std::vector<Song_t> getAllSongs() const;
    int createSong(const Song_t*);

private:
    Database();
    void getSongByQuery_(const char*, Song_t* __restrict__) const;

    sqlite3* m_db = 0;
    static Database* s_instance;
    const static char DB_NAME[];
};

} // namespace elevation


#endif // DATABASE_DATABASE_HPP
