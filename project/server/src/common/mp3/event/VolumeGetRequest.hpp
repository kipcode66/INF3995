#ifndef MP3_EVENT_VOLUMEGETREQUEST_HPP
#define MP3_EVENT_VOLUMEGETREQUEST_HPP

#include "AbstractPayloadlessEvent.hpp"

namespace elevation {

/**
 * @brief #Mp3Event which, when received, is interpreted
 * as a request to get the volume data. The receiver then
 * obtains the data and sends back a #VolumeGetResponse
 * event containing it.
 */
class VolumeGetRequest : public AbstractPayloadlessEvent {
public:
    explicit VolumeGetRequest();
    virtual ~VolumeGetRequest() = default;

    virtual void acceptVisitor(AbstractMp3EventVisitor& visitor) override;
};

} // namespace elevation

#endif // !MP3_EVENT_VOLUMEGETREQUEST_HPP
