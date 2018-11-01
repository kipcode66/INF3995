#include "PendingSongs.hpp"

#include <algorithm>
#include <iostream>

#include "exception/Terminate.hpp"

namespace elevation {

PendingSongs::PendingSongs(std::size_t maxSongs)
    : m_songList(maxSongs)
    , m_startFuture(m_startPromise.get_future())
    , m_terminateRequested(false)
{
    m_playerThread = std::thread(&PendingSongs::songStarter_, this);
}

PendingSongs::~PendingSongs() {
    m_terminateRequested.store(true);
    try {
        sendTerminate_();
        stopSong_();
        m_playerThread.join();
    }
    catch (std::system_error& e) { }
}

void PendingSongs::addSong(const std::experimental::filesystem::path& songPath) {
    std::lock_guard<std::mutex> lock(m_songListMutex);
    m_songList.push(songPath);
    if (m_songList.size() == 1) {
        m_startPromise.set_value();
    }
}

void PendingSongs::removeSong(const std::experimental::filesystem::path& songPath) {
    if (m_songList.indexOf(songPath) == 0) {
        m_startPromise.set_value();
    }
    m_songList.remove(songPath);
}

void PendingSongs::reorderSongs(std::size_t songAPosition, std::size_t songBPosition) {
    m_songList.swap(songAPosition, songBPosition);
}

void PendingSongs::songStarter_() {
    while (!m_terminateRequested.load()) {
        try {
            m_songListMutex.lock();
            // Using while() prevents the race condition where the only song is deleted
            // before we could lock the mutex again, but after the wakeup signal has been
            // sent.
            while (m_songList.size() == 0 && !m_terminateRequested.load()) {
                m_songListMutex.unlock();
                m_startFuture.get(); // May throw Terminate.
                m_startPromise = std::promise<void>();
                m_startFuture = m_startPromise.get_future();
                m_songListMutex.lock();
            }

            if (!m_terminateRequested.load()) {
                std::experimental::filesystem::path nextSongPath = m_songList.popNext();
                m_songListMutex.unlock();
                m_player.startPlaying(nextSongPath.string());
                m_player.waitUntilSongFinished();
            }
            else {
                m_songListMutex.unlock();
            }
        }
        catch (Terminate& e) { }
        catch (std::exception& e) {
            std::cerr << "SongList got C++ exeption : " << e.what() << std::endl;
        }
    }
}

void PendingSongs::stopSong_() {
    m_player.stopPlaying();
    m_player.waitUntilSongFinished();
}

void PendingSongs::sendTerminate_() {
    m_terminateRequested.store(true);
    m_startPromise.set_exception(std::make_exception_ptr(Terminate()));
}

} // namespace elevation
