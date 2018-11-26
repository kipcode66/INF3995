#include "HttpsServerEventFacade.hpp"

namespace elevation {

HttpsServerEventFacade::HttpsServerEventFacade(Logger& logger, std::shared_ptr<Mp3EventSocket> socket)
    : m_volumeGetRequestAdapter(new HttpsServerVolumeGetRequestAdapter(logger, socket))
    , m_manager(HttpsServerMp3EventVisitor(logger, m_volumeGetRequestAdapter), socket, logger)
{ }

VolumeData_t HttpsServerEventFacade::getVolumeData() {
    return m_volumeGetRequestAdapter->getVolumeData();
}

} // namespace elevation
