#include "Mp3Event.hpp"

#include <sstream>

namespace elevation {

Mp3Event::Mp3Event(EventType eventType)
{
    std::size_t eventTypeAsInteger = static_cast<std::size_t>(eventType);
    if (eventTypeAsInteger <= 0 || eventTypeAsInteger >= static_cast<std::size_t>(EventType::NUM_EVENTS)) {
        throw std::invalid_argument("Cannot create event " + std::to_string(eventTypeAsInteger) + " as it is not a valid event type");
    }
    m_eventType = eventType;
}

Mp3Event::~Mp3Event() { }

std::string Mp3Event::serialize() const {
    std::ostringstream messageStream;
    messageStream << serializeElement(EVENT_SIGNATURE) << serializeElement(m_eventType);
    return messageStream.str();
}

void Mp3Event::padToDataSize(std::ostream& dataStream) {
    std::streampos initialWritePos = dataStream.tellp();
    dataStream.seekp(0, std::ios::end);
    std::size_t currentDataSize = dataStream.tellp();
    dataStream.seekp(initialWritePos);

    while (currentDataSize < DATA_SIZE) {
        dataStream << PADDING;
        ++currentDataSize;
    }
}

} // namespace elevation
