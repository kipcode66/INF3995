#ifndef MP3_EVENT_HTTPSSERVEREVENTMANAGER_HPP
#define MP3_EVENT_HTTPSSERVEREVENTMANAGER_HPP

#include <thread>

#include <common/logger/Logger.hpp>
#include <common/mp3/communication/Mp3EventListenerSocket.hpp>
#include <common/mp3/communication/Mp3EventSocket.hpp>

#include "HttpsServerMp3EventVisitor.hpp"

namespace elevation {

/**
 * @brief Calls the HttpsServerEventVisitor when socket events are
 * read from the given socket.
 */
class HttpsServerEventManager {
public:
    explicit HttpsServerEventManager(HttpsServerMp3EventVisitor eventVisitor, std::shared_ptr<Mp3EventSocket> socket, Logger& logger);
    virtual ~HttpsServerEventManager();

protected:
    static void readerThread_(HttpsServerMp3EventVisitor eventVisitor, std::shared_ptr<Mp3EventSocket> socket, Logger& logger);

protected:
    std::thread m_readerThread;
};

} // namespace elevation

#endif // !MP3_EVENT_HTTPSSERVEREVENTMANAGER_HPP
