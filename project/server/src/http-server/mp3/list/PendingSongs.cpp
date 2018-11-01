#include "PendingSongs.hpp"

#include <algorithm>

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
    auto it = std::find(m_pendingSongs.begin(), m_pendingSongs.end(), songPath);
    if (it != m_pendingSongs.end()) {
        m_pendingSongs.remove(songPath);
    }
    else {
        throw std::out_of_range("Cannot remove song : No song under path " + songPath.string());
    }
}

void PendingSongs::reorderSongs(std::size_t songAPosition, std::size_t songBPosition) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (songAPosition < m_pendingSongs.size() && songBPosition < m_pendingSongs.size()) {
        auto songA = m_pendingSongs.begin();
        auto songB = m_pendingSongs.begin();
        std::advance(songA, songAPosition);
        std::advance(songB, songBPosition);
        std::iter_swap(songA, songB);
    }
    else {
        throw std::out_of_range("Cannot swap songs : out of bounds.");
    }
}

} // namespace elevation
