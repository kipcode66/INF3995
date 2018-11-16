#include "VolumeChangeEvent.hpp"

#include <sstream>
#include <bitset>

namespace elevation {

VolumeChangeEvent::VolumeChangeEvent(uint8_t changeTo)
    : Mp3Event(Mp3Event::EventType::VOLUME_CHANGE)
{ }

VolumeChangeEvent::~VolumeChangeEvent() { }

std::string VolumeChangeEvent::serialize() const {
    std::ostringstream serializationStream;
    serializationStream << Mp3Event::serialize() << serializeInteger(m_changeTo);
    padToDataSize(serializationStream);
    return serializationStream.str();
}

} // namespace elevation
