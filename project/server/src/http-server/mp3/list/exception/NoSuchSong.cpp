#include "NoSuchSong.hpp"

namespace elevation {

const std::string NoSuchSong::MESSAGE_PREFIX = "NoSuchSong : ";

NoSuchSong::NoSuchSong(const std::string& message)
    : m_message(message)
{ }

const char* NoSuchSong::what() const noexcept {
    return (MESSAGE_PREFIX + m_message).c_str();
}

} // namespace elevation
