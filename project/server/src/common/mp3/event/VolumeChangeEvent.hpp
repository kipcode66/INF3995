#ifndef MP3_EVENT_VOLUMECHANGEEVENT_HPP
#define MP3_EVENT_VOLUMECHANGEEVENT_HPP

#include "mp3/definitions/Volume.hpp"

#include "Mp3Event.hpp"

namespace elevation {

class VolumeChangeEvent : public Mp3Event {
public:
    explicit VolumeChangeEvent(volumePercent_t changeTo);
    virtual ~VolumeChangeEvent();

    virtual bool operator==(const Mp3Event& other) const override;

    virtual std::string serialize() const;
    volumePercent_t getNewVolume() const { return m_changeTo; }

    virtual void acceptVisitor(const AbstractMp3EventVisitor& visitor) override;

protected:
    volumePercent_t m_changeTo;
};

} // namespace elevation

#endif // !MP3_EVENT_VOLUMECHANGEEVENT_HPP
