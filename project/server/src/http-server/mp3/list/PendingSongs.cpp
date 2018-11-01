#include "PendingSongs.hpp"

namespace elevation {

const std::experimental::filesystem::path PendingSongs::SONGS_DIR = ".";

PendingSongs::PendingSongs(std::size_t maxSongs) {
    m_maxSongs = maxSongs;
}

PendingSongs::~PendingSongs() { }

void PendingSongs::addSong(const std::string& songName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_pendingSongs.size() < m_maxSongs) {
        m_pendingSongs.push_back(SONGS_DIR / songName);
    }
    else {
        throw std::out_of_range("Cannot add a new song : Already at full capacity.");
    }
}

void PendingSongs::removeSong(const std::string& songName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_pendingSongs.remove(SONGS_DIR / songName);
}

void PendingSongs::reorderSong(std::size_t songAPosition, std::size_t songBPosition) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto songA = m_pendingSongs.begin();
    auto songB = m_pendingSongs.begin();
    std::advance(songA, songAPosition);
    std::advance(songB, songBPosition);
    std::swap(songA, songB);
}

} // namespace elevation
