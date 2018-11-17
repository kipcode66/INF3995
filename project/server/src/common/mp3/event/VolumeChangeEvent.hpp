#ifndef MP3_EVENT_VOLUMECHANGEEVENT_HPP
#define MP3_EVENT_VOLUMECHANGEEVENT_HPP

#include <inttypes.h>

#include "Mp3Event.hpp"

namespace elevation {

class VolumeChangeEvent : public Mp3Event {
public:
    explicit VolumeChangeEvent(uint8_t changeTo);
    virtual ~VolumeChangeEvent();

    virtual std::string serialize() const;

    uint8_t getNewVolume() const { return m_changeTo; }

protected:
    uint8_t m_changeTo;
};

} // namespace elevation

#endif // !MP3_EVENT_VOLUMECHANGEEVENT_HPP
