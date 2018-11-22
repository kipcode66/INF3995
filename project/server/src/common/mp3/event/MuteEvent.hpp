#ifndef MP3_EVENT_MUTEEVENT_HPP
#define MP3_EVENT_MUTEEVENT_HPP

#include "Mp3Event.hpp"

namespace elevation {

class MuteEvent : public Mp3Event {
public:
    explicit MuteEvent();
    virtual ~MuteEvent() = default;
    virtual bool operator==(const Mp3Event& other) const;
    virtual std::string serialize() const;
    virtual void acceptVisitor(const AbstractMp3EventVisitor& visitor);
};

} // namespace elevation

#endif // !MP3_EVENT_MUTEEVENT_HPP
