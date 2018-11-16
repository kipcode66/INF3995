#ifndef MP3_EVENT_VOLUMECHANGEEVENT_HPP
#define MP3_EVENT_VOLUMECHANGEEVENT_HPP

#include <inttypes.h>

#include "Mp3Event.hpp"

namespace elevation {

class VolumeChangeEvent : public Mp3Event {
    static constexpr uint16_t EVENT_TYPE = 1; ///< Must be different for each event.

public:
    explicit VolumeChangeEvent(uint8_t changeTo);
    virtual ~VolumeChangeEvent();

    virtual std::string serialize() const;

protected:
    uint8_t m_changeTo;
};

} // namespace elevation

#endif // !MP3_EVENT_VOLUMECHANGEEVENT_HPP
