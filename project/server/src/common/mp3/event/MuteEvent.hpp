#ifndef MP3_EVENT_MUTEEVENT_HPP
#define MP3_EVENT_MUTEEVENT_HPP

#include "AbstractPayloadlessEvent.hpp"

namespace elevation {

class MuteEvent : public AbstractPayloadlessEvent {
public:
    explicit MuteEvent();
    virtual ~MuteEvent();

    virtual void acceptVisitor(AbstractMp3EventVisitor& visitor);
};

} // namespace elevation

#endif // !MP3_EVENT_MUTEEVENT_HPP
