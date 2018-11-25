#include "Mp3Player.hpp"

#include <iostream>
#include <functional>

#include <common/mp3/header/Mp3Header.hpp>

#include "mp3/player/mad/MadDecoder.hpp"
#include "mp3/player/mad/MadAudioFormatter.hpp"
#include "mp3/player/pulse/PulseDevice.hpp"

namespace elevation {

void Mp3Player::run_(std::string fileName, std::shared_ptr<std::atomic<bool>> stopRequested) {
    try {
        MadDecoder decoder(std::move(SharedFileMemory(fileName)), MadAudioFormatter());
        uint32_t rate = PulseDevice::DEFAULT_SAMPLE_RATE;
        try {
            Mp3Header header{fileName};
            rate = header.getSampleRate();
        }
        catch (...) { }
        PulseDevice pulse{rate};

        while (!decoder.isDone() && !stopRequested->load()) {
            std::vector<uint8_t> frame = decoder.decodeNextFrame();
            pulse.play(std::move(frame));
        }
    }
    catch (const std::exception& e) {
        std::cout << "MP3 player crashed with error : \"" << e.what() << '"' << std::endl;
    }
}

Mp3Player::Mp3Player()
    : m_player(defaultFuture_())
    , m_playerMutex(new std::mutex())
    , m_stopRequested(new std::atomic<bool>(false))
{ }

Mp3Player::Mp3Player(Mp3Player&& that)
    : m_stopRequested(std::move(that.m_stopRequested))
{
    this->m_playerMutex = std::move(that.m_playerMutex);
    std::lock_guard<std::mutex> lock(*m_playerMutex);
    this->m_player = std::move(that.m_player);
}

Mp3Player::~Mp3Player() { }

Mp3Player& Mp3Player::operator=(Mp3Player&& that) {
    this->m_stopRequested->store(true);
    this->m_stopRequested = std::move(that.m_stopRequested);
    this->m_playerMutex = std::move(that.m_playerMutex);
    std::lock_guard<std::mutex> lock(*m_playerMutex);
    this->m_player = std::move(that.m_player);
    return *this;
}

void Mp3Player::startPlaying(const std::string& fileName) {
    std::lock_guard<std::mutex> lock(*m_playerMutex);
    m_player.get();
    m_player = std::async(&Mp3Player::run_, fileName, m_stopRequested);
}

void Mp3Player::stopPlaying() {
    m_stopRequested->store(true);
    waitUntilSongFinished();
}

void Mp3Player::waitUntilSongFinished() {
    std::lock_guard<std::mutex> lock(*m_playerMutex);
    m_player.get();
    m_player = defaultFuture_();
}

std::future<void> Mp3Player::defaultFuture_() {
    return std::async([](){});
}

} // namespace elevation
