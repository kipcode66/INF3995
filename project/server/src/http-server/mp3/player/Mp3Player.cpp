#include "Mp3Player.hpp"

#include <iostream>
#include <functional>

#include "mp3/player/mad/MadDecoder.hpp"
#include "mp3/player/mad/MadAudioFormatter.hpp"
#include "mp3/player/pulse/PulseDevice.hpp"

namespace elevation {

void Mp3Player::run_(std::string fileName, std::shared_ptr<std::atomic<bool>> stopRequested) {
    try {
        MadDecoder decoder(std::move(SharedFileMemory(fileName)), MadAudioFormatter());
        PulseDevice pulse;

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
    , m_stopRequested(new std::atomic<bool>(false))
{ }

Mp3Player::Mp3Player(Mp3Player&& that)
    : m_player(std::move(that.m_player))
    , m_stopRequested(std::move(that.m_stopRequested))
{ }

Mp3Player::~Mp3Player() { }

Mp3Player& Mp3Player::operator=(Mp3Player&& that) {
    this->m_player = std::move(that.m_player);
    this->m_stopRequested->store(true);
    this->m_stopRequested = std::move(that.m_stopRequested);
    return *this;
}

void Mp3Player::startPlaying(const std::string& fileName) {
    m_player.get();
    m_player = std::async(&Mp3Player::run_, fileName, m_stopRequested);
}

void Mp3Player::stopPlaying() {
    m_stopRequested->store(true);
    waitUntilSongFinished();
}

void Mp3Player::waitUntilSongFinished() {
    m_player.get();
    m_player = defaultFuture_();
}

std::future<void> Mp3Player::defaultFuture_() {
    return std::async([](){});
}

} // namespace elevation
