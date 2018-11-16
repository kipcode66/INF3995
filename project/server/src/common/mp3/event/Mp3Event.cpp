#include "Mp3Event.hpp"

#include <sstream>

namespace elevation {

Mp3Event::Mp3Event() { }
Mp3Event::~Mp3Event() { }

std::string Mp3Event::serialize() const {
    std::ostringstream messageStream;
    messageStream << std::bitset<sizeof(EVENT_SIGNATURE)>(EVENT_SIGNATURE);
    return messageStream.str();
}

} // namespace elevation
