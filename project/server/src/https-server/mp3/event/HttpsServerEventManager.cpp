#include "HttpsServerEventManager.hpp"

#include <functional>
#include <iostream>

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>
#include <common/os/signal/SignalHandling.hpp>

#include "HttpsServerMp3EventVisitor.hpp"

namespace elevation {

HttpsServerEventManager::HttpsServerEventManager(HttpsServerMp3EventVisitor eventVisitor, std::shared_ptr<Mp3EventSocket> socket, Logger& logger)
{
    m_readerThread = std::thread(
        &HttpsServerEventManager::readerThread_,
        std::make_shared<HttpsServerMp3EventVisitor>(std::move(eventVisitor)),
        std::move(socket),
        std::ref(logger)
    );
}

HttpsServerEventManager::~HttpsServerEventManager() {
    // NOTE: In theory, since the HTTPS Server never finishes until we kill it,
    // this destructor should never get called. However, if it is, it will block forever.
    m_readerThread.join();
}

void HttpsServerEventManager::readerThread_(std::shared_ptr<HttpsServerMp3EventVisitor> eventVisitor, std::shared_ptr<Mp3EventSocket> socket, Logger& logger) {
    while (true) {
        try {
            socket->readEvent()->acceptVisitor(*eventVisitor);
        }
        catch (const std::exception& e) {
            logger.err(std::string("HttpsServerEventManager got C++ exception: ") + e.what());
        }
    }
}

} // namespace elevation
