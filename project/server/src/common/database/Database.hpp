#include <sqlite3/sqlite3.h>
#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <experimental/filesystem>
#include <misc/Statistics.hpp>

#include "Statement.hpp"
#include "templates/User.hpp"
#include "templates/Song.hpp"
#include "sqlite_error.hpp"
#include "Query.hpp"


namespace elevation {

class Database {
public:
    static constexpr const char*   ADMIN_NAME         = "admin";
    static constexpr const char*   DEFAULT_PASSWORD   = "admin";
    static constexpr const char*   TODAY_QUERY        = " WHERE timestamp BETWEEN julianday('now', 'start of day') AND julianday('now', 'start of day', '+1 day', '-1 second');";
    static constexpr const int32_t IS_BLACKLISTED     = 1;
    static constexpr const int32_t DEFAULT_SONG_ORDER = 0;

    static Database* instance();
    static void assertSqliteOk(int errcode, const std::string& message = "");

protected:
    static Database* s_instance;
    static const char DB_NAME[];

public:
    User_t getUserByMac     (const std::string&) const;
    User_t getUserById      (uint32_t) const;
    void   createUser       (const User_t* user);
    void   connectUser      (const struct User_t* user);
    bool   isUserConnected  (const uint32_t userId) const; 
    void   updateTimestamp  (const User_t* user);
    void   setAdminPassword (const std::string& password);
    void   connectAdmin     (const std::string& login, uint32_t adminId);
    void   disconnectAdmin  (uint32_t adminId);
    bool   isAdminConnected (uint32_t adminId) const;
    std::string getAdminPassword() const;
    std::pair<std::string, std::string> getSaltAndHashedPasswordByLogin(const std::string& login) const;

    Song_t              getSongById(int) const;
    Song_t              getSongByTitle(const std::string&) const;
    Song_t              getSongByPath(const std::string&) const;
    std::vector<Song_t> getSongsByUser(int userId) const;
    std::vector<Song_t> getAllSongs() const;
    std::vector<Song_t> getDailySongs() const;
    
    Statistics          getStatistics() const;

    void                createSong(const Song_t*);
    void                removeSong(uint32_t);

    void initDefaultAdmin();

    std::vector<User_t> getBlackList();

protected:
    Database();
    Database(std::experimental::filesystem::path);
    virtual ~Database();

    void executeAndRetryOnLock_(const Query&);
    void enableForeignKeys_();
    void wipeDbSongs_();

    Song_t getSongByQuery_(const Query&) const;
    std::vector<Song_t> getSongsByQuery_(const Query&) const;
    Song_t getSongFromStatement_(const Statement& stmt) const;

    User_t getUserByQuery_(const Query&) const;
    std::vector<User_t> getUsersByQuery_(const Query&) const;
    User_t getUserFromStatement_(const Statement& stmt) const;
    
    int getDailyUserCount_() const;
    int getDailySongCount_() const;
    int getDeletedSongsCount_() const;
    int getAverageSongDuration_() const;
    int getStatisticsFromQuery_(const Query&) const;
    
    void executeQuery_(const Query& query);

    sqlite3* m_db = 0;
};

} // namespace elevation


#endif // DATABASE_DATABASE_HPP
