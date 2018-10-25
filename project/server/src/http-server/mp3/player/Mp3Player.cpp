#include "Mp3Player.hpp"

#include <iostream>
#include <functional>
#include <chrono>

namespace elevation {

void Mp3Player::run_(std::string fileName) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    setIsPlayingTo_(true);
    std::cout << __PRETTY_FUNCTION__ << ':' << fileName << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    setIsPlayingTo_(false);
}

Mp3Player::Mp3Player()
{
    m_isPlaying = false;
}

Mp3Player::~Mp3Player() { }

void Mp3Player::startPlaying(const std::string& fileName) {
    {
        std::lock_guard<std::mutex> lock(m_isPlayingMutex);
        if (m_isPlaying) {
            throw std::runtime_error("Elevation MP3 player cannot start playing MP3 : Already playing");
        }
        m_playerThread = std::thread(&Mp3Player::run_, this, fileName);

        // Lock is released here
    }
    waitUntilSongStarted_();
}

void Mp3Player::waitUntilSongFinished() {
    // Yes, doing this correctly is complicated, even in C++14.
    auto checkIfSongFinished = std::bind(std::logical_not<bool>(), std::bind(&Mp3Player::isSongPlaying_, this));
    std::unique_lock<std::mutex> lock(m_isPlayingMutex); // Create and lock the mutex.
    m_isPlayingCondition.wait(lock, checkIfSongFinished); // While the song isn't finished, wait.
                                                          // Note that this is not polling, but there are
                                                          // cases where we can wake up even if the condition
                                                          // isn't true.
    m_playerThread.detach();
}

void Mp3Player::waitUntilSongStarted_() {
    auto checkIfSongStarted = std::bind(&Mp3Player::isSongPlaying_, this);
    std::unique_lock<std::mutex> lock(m_isPlayingMutex);
    m_isPlayingCondition.wait(lock, checkIfSongStarted);
}

void Mp3Player::setIsPlayingTo_(bool value) {
    {
        std::lock_guard<std::mutex> lock(m_isPlayingMutex);
        m_isPlaying = value;

        // Lock is released here
    }
    m_isPlayingCondition.notify_all();
}

bool Mp3Player::isSongPlaying_() {
    return m_isPlaying;
}

} // namespace elevation
