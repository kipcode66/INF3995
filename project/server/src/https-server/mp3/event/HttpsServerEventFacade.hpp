#ifndef MP3_EVENT_HTTPSSERVEREVENTFACADE_HPP
#define MP3_EVENT_HTTPSSERVEREVENTFACADE_HPP

#include <common/logger/Logger.hpp>

#include "HttpsServerEventManager.hpp"
#include "HttpsServerMp3EventVisitor.hpp"
#include "HttpsServerVolumeGetRequestAdapter.hpp"

namespace elevation {

/**
 * @brief Instanciating and keeping the objects required by the management
 * of MP3 events was starting to get a little complex, so here is a class
 * that handles this.
 */
class HttpsServerEventFacade {
public:
    explicit HttpsServerEventFacade(Logger& logger, std::shared_ptr<Mp3EventSocket> socket);
    HttpsServerEventFacade(const HttpsServerEventFacade&) = delete;
    virtual ~HttpsServerEventFacade() = default;

    HttpsServerEventFacade& operator=(const HttpsServerEventFacade&) = delete;

    VolumeData_t getVolumeData();

protected:
    std::shared_ptr<HttpsServerVolumeGetRequestAdapter> m_volumeGetRequestAdapter;
    HttpsServerEventManager m_manager;
};

} // namespace elevation

#endif // !MP3_EVENT_HTTPSSERVEREVENTFACADE_HPP
