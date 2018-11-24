#include "HttpsServerEventManager.hpp"

#include <functional>
#include <iostream>

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>
#include <common/os/signal/SignalHandling.hpp>

#include "HttpsServerMp3EventVisitor.hpp"

namespace elevation {

HttpsServerEventManager::HttpsServerEventManager(std::shared_ptr<Mp3EventSocket> socket, Logger& logger)
{
    m_accepterThread = std::thread(&HttpsServerEventManager::readerThread_, std::move(socket), std::ref(logger));
}

HttpsServerEventManager::~HttpsServerEventManager() {
    // NOTE: In theory, since the HTTPS Server never finishes until we kill it,
    // this destructor should never get called. However, if it is, it will block forever.
    m_accepterThread.join();
}

void HttpsServerEventManager::readerThread_(std::shared_ptr<Mp3EventSocket> socket, Logger& logger) {
    HttpsServerMp3EventVisitor visitor{logger};
    while (true) {
        try {
            socket->readEvent()->acceptVisitor(visitor);
        }
        catch (const std::exception& e) {
            logger.err(std::string("HttpsServerEventManager got C++ exception: ") + e.what());
        }
    }
}

} // namespace elevation
