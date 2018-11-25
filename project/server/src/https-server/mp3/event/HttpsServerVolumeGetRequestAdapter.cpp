#include "HttpsServerVolumeGetRequestAdapter.hpp"

#include <common/mp3/event/VolumeGetRequest.hpp>
#include <common/mp3/event/VolumeGetResponse.hpp>

namespace elevation {

HttpsServerVolumeGetRequestAdapter::HttpsServerVolumeGetRequestAdapter(Logger& logger, std::shared_ptr<Mp3EventSocket> socket, std::shared_ptr<HttpsServerMp3EventVisitor> visitor)
    : m_socket(socket)
    , m_visitor(visitor)
{ }

VolumeData_t HttpsServerVolumeGetRequestAdapter::getVolumeData() {
    VolumeGetRequest request;
    m_socket->writeEvent(request);

    std::unique_lock<std::mutex> lock(m_volumeUpdateMutex);
    m_wasVolumeDataUpdated = false;
    m_volumeUpdateCondition.wait_for(lock, TIMEOUT_DELAY, [this]() { return m_wasVolumeDataUpdated; });

    if (!m_wasVolumeDataUpdated) {
        throw std::runtime_error("Timeout occurred while waiting for the elevation-player to respond to the VolumeGetRequest");
    }

    VolumeData_t volumeData = m_lastVolumeData;
    return volumeData;
}

void HttpsServerVolumeGetRequestAdapter::setVolumeData_(const VolumeData_t& volumeData) {
    std::lock_guard<std::mutex> lock(m_volumeUpdateMutex);
    m_lastVolumeData = volumeData;
    m_wasVolumeDataUpdated = true;
    m_volumeUpdateCondition.notify_all();
}

} // namespace elevation
