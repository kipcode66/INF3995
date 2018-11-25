#ifndef MP3_EVENT_HTTPSSERVERVOLUMEGETREQUESTADAPTER_HPP
#define MP3_EVENT_HTTPSSERVERVOLUMEGETREQUESTADAPTER_HPP

#include <common/mp3/definitions/Volume.hpp>

#include "HttpsServerEventManager.hpp"

namespace elevation {

/**
 * @brief Adapter which hides the implementation of performing
 * a volume get request.
 */
class HttpsServerVolumeGetRequestAdapter {
public:
    explicit HttpsServerVolumeGetRequestAdapter(Logger& logger, std::shared_ptr<Mp3EventSocket> socket);
    virtual ~HttpsServerVolumeGetRequestAdapter() = default;

    VolumeData_t getVolume();

protected:
    HttpsServerEventManager m_eventManager;
    std::shared_ptr<Mp3EventSocket> m_socket;
};

} // namespace elevation

#endif // !MP3_EVENT_HTTPSSERVERVOLUMEGETREQUESTADAPTER_HPP
