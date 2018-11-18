#ifndef MP3_EVENT_VOLUMECHANGEEVENT_HPP
#define MP3_EVENT_VOLUMECHANGEEVENT_HPP

#include <inttypes.h>

#include "Mp3Event.hpp"

namespace elevation {

class VolumeChangeEvent : public Mp3Event {
public:
    explicit VolumeChangeEvent(uint8_t changeTo);
    virtual ~VolumeChangeEvent();

    virtual bool operator==(const Mp3Event& other) const override;

    virtual std::string serialize() const;
    uint8_t getNewVolume() const { return m_changeTo; }

    virtual void acceptVisitor(const AbstractMp3EventVisitor& visitor) override;

protected:
    uint8_t m_changeTo;
};

} // namespace elevation

#endif // !MP3_EVENT_VOLUMECHANGEEVENT_HPP
