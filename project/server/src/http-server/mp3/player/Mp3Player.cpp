#include "Mp3Player.hpp"

#include <iostream>
#include <functional>
#include <chrono>

namespace elevation {

bool Mp3Player::run_(std::string fileName) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << __PRETTY_FUNCTION__ << ':' << fileName << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return true;
}

Mp3Player::Mp3Player()
    : m_player(std::async([](){return true;}))
{ }

Mp3Player::~Mp3Player() { }

void Mp3Player::startPlaying(const std::string& fileName) {
    m_player.get();
    m_player = std::async(&Mp3Player::run_, this, fileName);
}

void Mp3Player::waitUntilSongFinished() {
    m_player.get();
}

} // namespace elevation
