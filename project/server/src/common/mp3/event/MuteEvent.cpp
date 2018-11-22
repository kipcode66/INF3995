#include "MuteEvent.hpp"

namespace elevation {

MuteEvent::MuteEvent()
    : Mp3Event(EventType::MUTE)
{ }

bool MuteEvent::operator==(const Mp3Event& other) const {
    return other.getEventType() == EventType::MUTE;
}

std::string MuteEvent::serialize() const {
    return "";
}

void MuteEvent::acceptVisitor(const AbstractMp3EventVisitor& visitor) {

}

} // namespace elevation
