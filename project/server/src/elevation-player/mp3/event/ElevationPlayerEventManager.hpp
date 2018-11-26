#ifndef MP3_EVENT_ELEVATIONPLAYEREVENTMANAGER_HPP
#define MP3_EVENT_ELEVATIONPLAYEREVENTMANAGER_HPP

#include <thread>

#include <common/logger/Logger.hpp>
#include <common/mp3/communication/Mp3EventListenerSocket.hpp>
#include <common/mp3/communication/Mp3EventSocket.hpp>

namespace elevation {

class ElevationPlayerEventManager {
public:
    explicit ElevationPlayerEventManager(uint16_t port, Logger& logger);
    virtual ~ElevationPlayerEventManager();

protected:
    static void accepterThread_(Mp3EventListenerSocket listener, Logger& logger);
    static void connectionThread_(std::shared_ptr<Mp3EventSocket> socket, Logger& logger);

protected:
    std::thread m_accepterThread;
};

} // namespace elevation

#endif // !MP3_EVENT_ELEVATIONPLAYEREVENTMANAGER_HPP
