#include "PendingSongs.hpp"

#include <algorithm>
#include <iostream>

#include "exception/Terminate.hpp"

namespace elevation {

PendingSongs::PendingSongs(
    std::function<path()> nextSongGetter,
    std::function<void(path)> songRemover
)
    : m_startPromise()
    , m_startFuture(m_startPromise.get_future())
    , m_nextSongGetter(nextSongGetter)
    , m_songRemover(songRemover)
    , m_terminateRequested(false)
{
    m_playerThread = std::thread(&PendingSongs::songStarter_, this);
}

PendingSongs::~PendingSongs() {
    try {
        sendTerminate_();
        stopSong();
        m_playerThread.join();
    }
    catch (std::system_error& e) { }
}

void PendingSongs::songStarter_() {
    while (!m_terminateRequested.load()) {
        try {
            path song(m_nextSongGetter());
            while (song == "" && !m_terminateRequested.load()) {
                std::this_thread::sleep_for(NEXT_SONG_POLLING_DELAY);
                song = m_nextSongGetter();
            }

            if (!m_terminateRequested.load()) {
                m_player.startPlaying(song);
                m_startPromise.set_value();
                m_player.waitUntilSongFinished();
                {
                    std::lock_guard<std::mutex> lock(m_startMutex);
                    m_startPromise = std::promise<void>();
                    m_startFuture = m_startPromise.get_future();
                } // m_startMutex gets unlocked here.
            }
            if (song != "") {
                m_songRemover(song);
            }
        }
        catch (Terminate& e) { }
        catch (std::exception& e) {
            std::cerr << "PendingSongs got C++ exeption : " << e.what() << std::endl;
        }
    }
}

void PendingSongs::waitUntilSongStarted() {
    m_startFuture.get();
}

void PendingSongs::waitUntilSongFinished() {
    m_player.waitUntilSongFinished();
}

void PendingSongs::stopSong() {
    m_player.stopPlaying();
    m_player.waitUntilSongFinished();
}

void PendingSongs::sendTerminate_() {
    m_terminateRequested.store(true);
}

} // namespace elevation
