#ifndef MP3_EVENT_MUTEEVENT_HPP
#define MP3_EVENT_MUTEEVENT_HPP

#include "AbstractPayloadlessEvent.hpp"

namespace elevation {

class MuteEvent : public AbstractPayloadlessEvent {
public:
    explicit MuteEvent();
    virtual ~MuteEvent() = default;
    virtual bool operator==(const Mp3Event& other) const;
    virtual std::string serialize() const;
    virtual void acceptVisitor(const AbstractMp3EventVisitor& visitor);
};

} // namespace elevation

#endif // !MP3_EVENT_MUTEEVENT_HPP
