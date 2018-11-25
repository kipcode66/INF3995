#include "HttpsServerVolumeGetRequestAdapter.hpp"

#include <common/mp3/event/VolumeGetRequest.hpp>
#include <common/mp3/event/VolumeGetResponse.hpp>

namespace elevation {

HttpsServerVolumeGetRequestAdapter::HttpsServerVolumeGetRequestAdapter(Logger& logger, std::shared_ptr<Mp3EventSocket> socket)
    : m_eventManager(socket, logger)
    , m_socket(socket)
{ }

VolumeData_t HttpsServerVolumeGetRequestAdapter::getVolume() {
    VolumeGetRequest request;
    VolumeData_t volumeData = {.volume = 50, .isMuted = false};
    return volumeData;
}

} // namespace elevation
