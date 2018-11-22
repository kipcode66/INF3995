#include "MuteEvent.hpp"

namespace elevation {

MuteEvent::MuteEvent()
    : AbstractPayloadlessEvent(EventType::MUTE)
{ }

void MuteEvent::acceptVisitor(const AbstractMp3EventVisitor& visitor) {
    visitor.onMuteEvent(*this);
}

} // namespace elevation
