#ifndef MP3_EVENT_VOLUMEGETRESPONSE_HPP
#define MP3_EVENT_VOLUMEGETRESPONSE_HPP

#include "Mp3Event.hpp"
#include "mp3/definitions/Volume.hpp"

namespace elevation {

class VolumeGetResponse : public Mp3Event {
public:
    explicit VolumeGetResponse(const VolumeData_t& volumeData);
    virtual ~VolumeGetResponse() = default;

    virtual bool operator==(const Mp3Event& other) const override;

    virtual std::string serialize() const override;

    virtual void acceptVisitor(AbstractMp3EventVisitor& visitor) override;

    VolumeData_t getVolumeData() const { return m_volumeData; }

protected:
    VolumeData_t m_volumeData;
};

} // namespace elevation

#endif // !MP3_EVENT_VOLUMEGETRESPONSE_HPP
