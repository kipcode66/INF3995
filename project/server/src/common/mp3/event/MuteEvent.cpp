#include "MuteEvent.hpp"

namespace elevation {

MuteEvent::MuteEvent()
    : AbstractPayloadlessEvent(EventType::MUTE)
{ }

MuteEvent::~MuteEvent() { }

void MuteEvent::acceptVisitor(const AbstractMp3EventVisitor& visitor) {
    visitor.onMuteEvent(*this);
}

} // namespace elevation
