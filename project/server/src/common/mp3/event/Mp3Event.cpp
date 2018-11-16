#include "Mp3Event.hpp"

#include <sstream>

namespace elevation {

Mp3Event::Mp3Event(EventType eventType)
    : m_eventType(eventType)
{ }

Mp3Event::~Mp3Event() { }

std::string Mp3Event::serialize() const {
    std::ostringstream messageStream;
    messageStream << serializeInteger(EVENT_SIGNATURE) << std::bitset<sizeof(m_eventType)>(static_cast<std::size_t>(m_eventType));
    return messageStream.str();
}

void Mp3Event::padToDataSize(std::ostream& dataStream) {
    std::streampos initialWritePos = dataStream.tellp();
    dataStream.seekp(0, std::ios::end);
    std::size_t currentDataSize = dataStream.tellp();
    dataStream.seekp(initialWritePos);

    while (currentDataSize < DATA_SIZE) {
        dataStream << PADDING;
    }
}

} // namespace elevation
