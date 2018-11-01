#include "SongList.hpp"

#include <algorithm>

namespace elevation {

SongList::SongList(std::size_t maxSongs) {
    m_maxSongs = maxSongs;
}

SongList::~SongList() { }

void SongList::push(const std::experimental::filesystem::path& songPath) {
    std::lock_guard<std::mutex> lock(m_songListMutex);
    if (m_songs.size() < m_maxSongs) {
        m_songs.push_back(songPath);
    }
    else {
        throw std::out_of_range("Cannot add a new song : Already at full capacity.");
    }
}

void SongList::remove(const std::experimental::filesystem::path& songPath) {
    std::lock_guard<std::mutex> lock(m_songListMutex);
    auto it = std::find(m_songs.begin(), m_songs.end(), songPath);
    if (it != m_songs.end()) {
        m_songs.remove(songPath);
    }
    else {
        throw std::out_of_range("Cannot remove song : No song under path " + songPath.string());
    }
}

void SongList::swap(std::size_t songAPosition, std::size_t songBPosition) {
    std::lock_guard<std::mutex> lock(m_songListMutex);
    if (songAPosition < m_songs.size() && songBPosition < m_songs.size()) {
        auto songA = m_songs.begin();
        auto songB = m_songs.begin();
        std::advance(songA, songAPosition);
        std::advance(songB, songBPosition);
        std::iter_swap(songA, songB);
    }
    else {
        throw std::out_of_range("Cannot swap songs : out of bounds.");
    }
}

} // namespace elevation
