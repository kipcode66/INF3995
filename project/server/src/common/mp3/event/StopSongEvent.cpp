#include "StopSongEvent.hpp"

namespace elevation {

StopSongEvent::StopSongEvent()
    : AbstractPayloadlessEvent(EventType::STOP_SONG)
{ }

StopSongEvent::~StopSongEvent() { }

void StopSongEvent::acceptVisitor(AbstractMp3EventVisitor& visitor) {
    visitor.onStopSongEvent(*this);
}

} // namespace elevation
