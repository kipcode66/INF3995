#include "MuteEvent.hpp"

namespace elevation {

MuteEvent::MuteEvent()
    : Mp3Event(EventType::MUTE)
{ }

bool MuteEvent::operator==(const Mp3Event& other) const {
    return other.getEventType() == EventType::MUTE;
}

std::string MuteEvent::serialize() const {
    std::ostringstream serializationStream;
    serializationStream << Mp3Event::serialize();
    Mp3Event::padToDataSize(serializationStream);
    std::string serialization = serializationStream.str();
    return serialization;
}

void MuteEvent::acceptVisitor(const AbstractMp3EventVisitor& visitor) {
    visitor.onMuteEvent(*this);
}

} // namespace elevation
