#include "Mp3Player.hpp"

#include <iostream>
#include <functional>

namespace elevation {

void Mp3Player::run_(std::string fileName) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << __PRETTY_FUNCTION__ << ':' << fileName << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

Mp3Player::Mp3Player()
    : m_player(defautFuture_())
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
}

std::future<void> Mp3Player::defautFuture_() {
    return std::async([](){});
}

} // namespace elevation
