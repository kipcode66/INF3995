#include <database/Database.hpp>
#include <thread>
#include <chrono>
#include <sqlite3/sqlite3.h>
#include <experimental/filesystem>

#include "Mp3AutoPlayerCallbacks.hpp"

using namespace elevation;
using namespace std::chrono_literals;
namespace fs = std::experimental::filesystem;

Mp3AutoPlayerCallbacks::Mp3AutoPlayerCallbacks(
        Logger& logger,
        FileCache& cache)
    : m_logger(logger)
    , m_cache(cache)
    , m_autoPlayer(nullptr)
{
    m_autoPlayer = new Mp3AutoPlayer([this](){return this->newSongProvider_();},[this](fs::path p){this->songRemover_(p);});
}

Mp3AutoPlayerCallbacks::~Mp3AutoPlayerCallbacks() {
    delete m_autoPlayer;
}

Mp3AutoPlayer& Mp3AutoPlayerCallbacks::getReferenceToAutoPlayer() const {
    return *m_autoPlayer;
}

fs::path Mp3AutoPlayerCallbacks::newSongProvider_() const {
    fs::path newSong = Mp3AutoPlayer::NO_SONG;
    bool retry;
    do {
        retry = false;
        try {
            Database* db = Database::instance();
            auto songs = db->getAllSongs();
            if (songs.size() > 0) {
                newSong = songs[0].path;
            }
        }
        catch (const sqlite_error& e) {
            if (e.code() == SQLITE_LOCKED) {
                retry = true;
                newSong = Mp3AutoPlayer::NO_SONG;
                std::this_thread::sleep_for(100ms);
            }
            else {
                m_logger.err(e.what());
            }
        }
    } while (retry);
    try {
        if (newSong != Mp3AutoPlayer::NO_SONG) {
            Database* db = Database::instance();
            auto song = db->getSongByPath(newSong.c_str());
            db->removeSong(song.id);
        }
    }
    catch (sqlite_error& e) {
        m_logger.err(e.what());
    }
    return newSong;
}

void Mp3AutoPlayerCallbacks::songRemover_(fs::path pathOfSong) {
    try {
        m_cache.deleteFile(pathOfSong.filename());
    }
    catch (std::experimental::filesystem::filesystem_error& e) {
        m_logger.err(e.what());
    }
}
