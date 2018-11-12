#include <database/Database.hpp>

#include "Mp3AutoPlayerCallbacks.hpp"

using namespace elevation;
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
    try {
        Database* db = Database::instance();
        auto songs = db->getAllSongs();
        if (songs.size() > 0) {
            newSong = songs[0].path;
        }
    }
    catch (sqlite_error& e) {
        m_logger.err(e.what());
    }
    return newSong;
}

void Mp3AutoPlayerCallbacks::songRemover_(fs::path pathOfSong) {
    try {
        Database* db = Database::instance();
        auto song = db->getSongByPath(pathOfSong.c_str());
        if (song.id > 0) {
            m_cache.deleteFile(pathOfSong.filename());
            db->removeSong(song.id);
        }
    }
    catch (sqlite_error& e) {
        m_logger.err(e.what());
    }
}
