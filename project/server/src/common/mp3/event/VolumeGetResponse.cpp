#include "VolumeGetResponse.hpp"

namespace elevation {

VolumeGetResponse::VolumeGetResponse(const VolumeData_t& volumeData)
    : Mp3Event(EventType::VOLUME_GET_RESPONSE)
    , m_volumeData(volumeData)
{ }

bool VolumeGetResponse::operator==(const Mp3Event& other) const {
    return m_eventType == other.getEventType();
}

std::string VolumeGetResponse::serialize() const {
    std::ostringstream serializationStream;
    serializationStream << Mp3Event::serialize();

    std::string serializedVolumeData = Mp3Event::serializeElement(m_volumeData.volume) + Mp3Event::serializeElement(m_volumeData.isMuted);
    serializationStream.write(serializedVolumeData.c_str(), serializedVolumeData.size());

    padToDataSize(serializationStream);

    std::string serializedData = serializationStream.str();
    return serializedData;
}

void VolumeGetResponse::acceptVisitor(AbstractMp3EventVisitor& visitor)  {
    visitor.onVolumeGetResponse(*this);
}

} // namespace elevation
