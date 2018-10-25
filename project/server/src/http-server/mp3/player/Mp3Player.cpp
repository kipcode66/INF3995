#include "Mp3Player.hpp"

#include <iostream>

namespace elevation {

void Mp3Player::run_(std::string fileName) {
    std::cout << __PRETTY_FUNCTION__ << ':' << fileName << std::endl;
}

Mp3Player::Mp3Player() { }

Mp3Player::~Mp3Player() { }

void Mp3Player::startPlaying(const std::string& fileName) {
    if (!m_playing.joinable()) {
        throw std::runtime_error("Elevation MP3 player cannot start playing MP3 : Already playing");
    }
    m_playing = std::thread(&Mp3Player::run_, fileName);
}

} // namespace elevation
