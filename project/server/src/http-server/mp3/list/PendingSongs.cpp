#include "PendingSongs.hpp"

#include <algorithm>
#include <iostream>

namespace elevation {

PendingSongs::PendingSongs(std::size_t maxSongs)
    : m_start(defaultFuture_())
    , m_terminateRequested(false)
{
    m_maxSongs = maxSongs;
}

PendingSongs::~PendingSongs() {
    m_terminateRequested.store(true);
    try {
        m_playerThread.join();
    }
    catch (std::system_error& e) { }
}

void PendingSongs::addSong(const std::experimental::filesystem::path& songPath) {
    std::lock_guard<std::mutex> lock(m_songListMutex);
    if (m_pendingSongs.size() < m_maxSongs) {
        m_pendingSongs.push_back(songPath);
    }
    else {
        throw std::out_of_range("Cannot add a new song : Already at full capacity.");
    }
}

void PendingSongs::removeSong(const std::experimental::filesystem::path& songPath) {
    std::lock_guard<std::mutex> lock(m_songListMutex);
    auto it = std::find(m_pendingSongs.begin(), m_pendingSongs.end(), songPath);
    if (it != m_pendingSongs.end()) {
        m_pendingSongs.remove(songPath);
    }
    else {
        throw std::out_of_range("Cannot remove song : No song under path " + songPath.string());
    }
}

void PendingSongs::reorderSongs(std::size_t songAPosition, std::size_t songBPosition) {
    std::lock_guard<std::mutex> lock(m_songListMutex);
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

void PendingSongs::songStarter_() {
    while (!m_terminateRequested.load()) {
        try {
            m_songListMutex.lock();
            // Using while() prevents the race condition where the only song is deleted
            // before we could lock the mutex again, but after the wakeup signal has been
            // sent.
            while (m_pendingSongs.size() == 0 && !m_terminateRequested.load()) {
                m_songListMutex.unlock();
                m_start.get();
                m_start = defaultFuture_();
                m_songListMutex.lock();
            }

            if (!m_terminateRequested.load()) {
                std::experimental::filesystem::path nextSongPath = m_pendingSongs.front();
                m_songListMutex.unlock();
                m_player.startPlaying(nextSongPath.string());
                m_player.waitUntilSongFinished();
            }
            else {
                m_songListMutex.unlock();
            }
        }
        catch (std::exception& e) {
            std::cerr << "SongList got C++ exeption : " << e.what() << std::endl;
        }
    }
}

void PendingSongs::stopSong_() {
    m_player.stopPlaying();
    m_player.waitUntilSongFinished();
}

std::future<void> PendingSongs::defaultFuture_() {
    return std::async([](){});
}

} // namespace elevation
