#include "Mp3Player.hpp"

#include <iostream>
#include <functional>

#include "mp3/player/mad/MadDecoder.hpp"
#include "mp3/player/mad/MadAudioFormatter.hpp"
#include "mp3/player/pulse/PulseDevice.hpp"

namespace elevation {

void Mp3Player::run_(std::string fileName) {
    try {
        MadDecoder::createInstance(SharedFileMemory(fileName), MadAudioFormatter());
        MadDecoder& decoder = MadDecoder::getInstance();
        PulseDevice pulse;

        while (true) {
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
{ }

Mp3Player::Mp3Player(Mp3Player&& that)
    : m_player(std::move(that.m_player))
{ }

Mp3Player& Mp3Player::operator=(Mp3Player&& that) {
    this->m_player = std::move(that.m_player);
    return *this;
}

Mp3Player::~Mp3Player() { }

void Mp3Player::startPlaying(const std::string& fileName) {
    m_player.get();
    m_player = std::async(&Mp3Player::run_, this, fileName);
}

void Mp3Player::waitUntilSongFinished() {
    m_player.get();
    m_player = defaultFuture_();
}

std::future<void> Mp3Player::defaultFuture_() {
    return std::async([](){});
}

} // namespace elevation
