#ifndef MP3_EVENT_HTTPSSERVERVOLUMEGETREQUESTADAPTER_HPP
#define MP3_EVENT_HTTPSSERVERVOLUMEGETREQUESTADAPTER_HPP

#include <mutex>
#include <condition_variable>
#include <chrono>

#include <common/mp3/definitions/Volume.hpp>

#include "HttpsServerEventManager.hpp"

namespace elevation {

class HttpsServerMp3EventVisitor;

/**
 * @brief Adapter which hides the implementation of performing
 * a volume get request.
 */
class HttpsServerVolumeGetRequestAdapter {

    friend class HttpsServerMp3EventVisitor; // Requires setVolumeData_().

public:
    static constexpr std::chrono::seconds TIMEOUT_DELAY{10};

public:
    explicit HttpsServerVolumeGetRequestAdapter(Logger& logger, std::shared_ptr<Mp3EventSocket> socket, HttpsServerMp3EventVisitor eventVisitor);
    virtual ~HttpsServerVolumeGetRequestAdapter() = default;

    VolumeData_t getVolumeData();

protected:
    void setVolumeData_(const VolumeData_t& volumeData);

protected:
    HttpsServerEventManager m_eventManager;
    std::shared_ptr<Mp3EventSocket> m_socket;

    bool m_wasVolumeDataUpdated;
    VolumeData_t m_lastVolumeData;
    std::condition_variable m_volumeUpdateCondition;
    std::mutex m_volumeUpdateMutex;
};

} // namespace elevation

#endif // !MP3_EVENT_HTTPSSERVERVOLUMEGETREQUESTADAPTER_HPP
