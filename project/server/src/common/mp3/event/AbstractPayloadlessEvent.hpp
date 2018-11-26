#ifndef MP3_EVENT_ABSTRACTPAYLOADLESSEVENT_HPP
#define MP3_EVENT_ABSTRACTPAYLOADLESSEVENT_HPP

#include "Mp3Event.hpp"

namespace elevation {

class AbstractPayloadlessEvent : public Mp3Event {
public:
    explicit AbstractPayloadlessEvent(EventType type);
    virtual ~AbstractPayloadlessEvent() = default;

    virtual bool operator==(const Mp3Event& other) const override;
    virtual std::string serialize() const override;
    virtual void acceptVisitor(AbstractMp3EventVisitor& visitor) override = 0;
};

} // namespace elevation

#endif // !MP3_EVENT_ABSTRACTPAYLOADLESSEVENT_HPP
