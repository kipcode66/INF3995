#ifndef MP3_EVENT_UNMUTEEVENT_HPP
#define MP3_EVENT_UNMUTEEVENT_HPP

#include "AbstractPayloadlessEvent.hpp"

namespace elevation {

class UnmuteEvent : public AbstractPayloadlessEvent {
public:
    explicit UnmuteEvent();
    virtual ~UnmuteEvent() = default;

    virtual void acceptVisitor(AbstractMp3EventVisitor& visitor) override;
};

} // namespace elevation

#endif // !MP3_EVENT_UNMUTEEVENT_HPP
