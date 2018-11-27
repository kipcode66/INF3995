#include <database/Database.hpp>
#include <thread>
#include <chrono>
#include <sqlite3/sqlite3.h>
#include <experimental/filesystem>
#include <string.h>

#include "Mp3AutoPlayerCallbacks.hpp"

using namespace elevation;
using namespace std::chrono_literals;
namespace fs = std::experimental::filesystem;

Mp3AutoPlayerCallbacks::Mp3AutoPlayerCallbacks(
        Logger& logger,
        FileCache& cache)
    : m_autoPlayer(nullptr)
{
    m_autoPlayer = std::shared_ptr<Mp3AutoPlayer>(new Mp3AutoPlayer(
        [this, &logger, &cache]() {
            return newSongProvider_(logger);
        },
        [this, &logger, &cache](fs::path p) {
            songRemover_(p, logger, cache);
        }
    ));
}

Mp3AutoPlayerCallbacks::~Mp3AutoPlayerCallbacks() {
    // automatic delete of m_autoPlayer when the shared_ptr has no more references
}

std::shared_ptr<Mp3AutoPlayer> Mp3AutoPlayerCallbacks::getReferenceToAutoPlayer() const {
    return m_autoPlayer;
}

fs::path Mp3AutoPlayerCallbacks::newSongProvider_(Logger& logger) {
    fs::path newSong = Mp3AutoPlayer::NO_SONG;
    bool retry;
    do {
        retry = false;
        try {
            Database* db = Database::instance();
            auto songs = db->getAllPlayableSongs();
            if (songs.size() > 0 && strlen(songs[0].path) > 0) {
                newSong = songs[0].path;
            }
        }
        catch (const sqlite_error& e) {
            if (e.code() == SQLITE_LOCKED ||
                e.code() == SQLITE_BUSY) {
                retry = true;
                newSong = Mp3AutoPlayer::NO_SONG;
                std::this_thread::sleep_for(100ms);
            }
            else {
                logger.err(e.what());
            }
        }
    } while (retry);

    return newSong;
}

void Mp3AutoPlayerCallbacks::songRemover_(fs::path pathOfSong, Logger& logger, FileCache& cache) {
    bool retry;
    do {
        retry = false;
        try {
            Database* db = Database::instance();
            Song_t oldSong = db->getSongByPath(pathOfSong);
            if (oldSong.id != 0) {
                db->removeSong(oldSong.id, true);
            }
            cache.deleteFile(pathOfSong.filename());
        }
        catch (sqlite_error& e) {
            if (e.code() == SQLITE_LOCKED ||
                e.code() == SQLITE_BUSY) {
                retry = true;
                std::this_thread::sleep_for(100ms);
            }
            else {
                logger.err(e.what());
            }
        }
        catch (fs::filesystem_error& e) {
            logger.err(e.what());
        }
    } while (retry);
}
