#include "VolumeGetRequest.hpp"

namespace elevation {

VolumeGetRequest::VolumeGetRequest()
    : AbstractPayloadlessEvent(EventType::VOLUME_GET_REQUEST)
{ }

void VolumeGetRequest::acceptVisitor(AbstractMp3EventVisitor& visitor) {
    visitor.onVolumeGetRequest(*this);
}

} // namespace elevation
