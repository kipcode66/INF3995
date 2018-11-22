#include "UnmuteEvent.hpp"

namespace elevation {

UnmuteEvent::UnmuteEvent()
    : AbstractPayloadlessEvent(EventType::UNMUTE)
{ }

void UnmuteEvent::acceptVisitor(const AbstractMp3EventVisitor& visitor) {

}

} // namespace elevation
