#include "PendingSongs.hpp"

namespace elevation {

PendingSongs::PendingSongs(std::size_t maxSongs) {
    m_maxSongs = maxSongs;
}

PendingSongs::~PendingSongs() { }

void PendingSongs::addSong(const std::experimental::filesystem::path& songPath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_pendingSongs.size() < m_maxSongs) {
        m_pendingSongs.push_back(songPath);
    }
    else {
        throw std::out_of_range("Cannot add a new song : Already at full capacity.");
    }
}

void PendingSongs::removeSong(const std::experimental::filesystem::path& songPath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_pendingSongs.remove(songPath);
}

void PendingSongs::reorderSong(std::size_t songAPosition, std::size_t songBPosition) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto songA = m_pendingSongs.begin();
    auto songB = m_pendingSongs.begin();
    std::advance(songA, songAPosition);
    std::advance(songB, songBPosition);
    if (songA != m_pendingSongs.end() && songB != m_pendingSongs.end()) {
        std::iter_swap(songA, songB);
    }
    else {
        throw std::out_of_range("Cannot swap songs : out of bounds.");
    }
}

} // namespace elevation
