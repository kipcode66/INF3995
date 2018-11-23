#include "UnmuteEvent.hpp"

namespace elevation {

UnmuteEvent::UnmuteEvent()
    : AbstractPayloadlessEvent(EventType::UNMUTE)
{ }

void UnmuteEvent::acceptVisitor(AbstractMp3EventVisitor& visitor) {
    visitor.onUnmuteEvent(*this);
}

} // namespace elevation
