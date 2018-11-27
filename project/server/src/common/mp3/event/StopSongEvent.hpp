#ifndef MP3_EVENT_STOPSONGEVENT_HPP
#define MP3_EVENT_STOPSONGEVENT_HPP

#include "AbstractPayloadlessEvent.hpp"

namespace elevation {

class StopSongEvent : public AbstractPayloadlessEvent {
public:
    explicit StopSongEvent();
    virtual ~StopSongEvent();

    virtual void acceptVisitor(AbstractMp3EventVisitor& visitor);
};

} // namespace elevation

#endif // !MP3_EVENT_STOPSONGEVENT_HPP
