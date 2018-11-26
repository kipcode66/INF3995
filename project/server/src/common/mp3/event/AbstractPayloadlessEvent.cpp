#include "AbstractPayloadlessEvent.hpp"

namespace elevation {

AbstractPayloadlessEvent::AbstractPayloadlessEvent(EventType type)
    : Mp3Event(type)
{ }

bool AbstractPayloadlessEvent::operator==(const Mp3Event& other) const {
    return m_eventType == other.getEventType();
}

std::string AbstractPayloadlessEvent::serialize() const {
    std::ostringstream serializationStream;
    serializationStream << Mp3Event::serialize();
    Mp3Event::padToDataSize(serializationStream);
    std::string serialization = serializationStream.str();
    return serialization;
}

} // namespace elevation
