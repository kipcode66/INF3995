#include "VolumeChangeEvent.hpp"

#include <sstream>
#include <bitset>

namespace elevation {

VolumeChangeEvent::VolumeChangeEvent(uint8_t changeTo) { }
VolumeChangeEvent::~VolumeChangeEvent() { }

std::string VolumeChangeEvent::serialize() const {
    std::ostringstream serializationStream;
    serializationStream << Mp3Event::EVENT_SIGNATURE << EVENT_TYPE << serializeInteger(m_changeTo);
    padToDataSize(serializationStream);
    return serializationStream.str();
}

} // namespace elevation
