#include "VolumeChangeEvent.hpp"

#include <sstream>
#include <bitset>

namespace elevation {

VolumeChangeEvent::VolumeChangeEvent(volumePercent_t changeTo)
    : Mp3Event(Mp3Event::EventType::VOLUME_CHANGE)
    , m_changeTo(changeTo)
{ }

VolumeChangeEvent::~VolumeChangeEvent() { }

std::string VolumeChangeEvent::serialize() const {
    std::ostringstream serializationStream;
    serializationStream << Mp3Event::serialize() << serializeElement(m_changeTo);
    padToDataSize(serializationStream);
    return serializationStream.str();
}

bool VolumeChangeEvent::operator==(const Mp3Event& other) const {
    const VolumeChangeEvent* otherVolumeChangeEvent = dynamic_cast<const VolumeChangeEvent*>(&other);
    if (otherVolumeChangeEvent == nullptr) {
        return false;
    }
    else {
        return m_changeTo == otherVolumeChangeEvent->m_changeTo;
    }
}

void VolumeChangeEvent::acceptVisitor(AbstractMp3EventVisitor& visitor) {
    visitor.onVolumeChangeEvent(*this);
}

} // namespace elevation
