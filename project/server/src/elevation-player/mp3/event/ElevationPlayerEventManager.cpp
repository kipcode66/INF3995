#include "ElevationPlayerEventManager.hpp"

#include <functional>
#include <iostream>

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>
#include <common/os/signal/SignalHandling.hpp>

#include "mp3/event/ElevationPlayerMp3EventVisitor.hpp"

namespace elevation {

ElevationPlayerEventManager::ElevationPlayerEventManager(uint16_t port, Logger& logger, std::shared_ptr<Mp3AutoPlayer> autoPlayer)
    : m_autoPlayer(autoPlayer)
{
    Mp3EventListenerSocket listener{port};
    m_accepterThread = std::thread(&ElevationPlayerEventManager::accepterThread_, std::move(listener), std::ref(logger));
}

ElevationPlayerEventManager::~ElevationPlayerEventManager() {
    // NOTE: In theory, since the Elevation Player never finishes until we kill it,
    // this destructor should never get called. However, if it is, it will block forever.
    m_accepterThread.join();
}

void ElevationPlayerEventManager::accepterThread_(Mp3EventListenerSocket listener, Logger& logger) {
    while (true) {
        try {
            std::unique_ptr<Socket> socket = listener.accept();
            std::shared_ptr<Mp3EventSocket> eventSocket{new Mp3EventSocket{std::move(*socket)}};
            std::thread(&ElevationPlayerEventManager::connectionThread_, std::move(eventSocket), std::ref(logger)).detach();
        }
        catch (const std::exception& e) {
            std::cerr << "A C++ exception occurred while trying to establish client-server connections : " <<
                e.what() << std::endl;
        }

        if (SignalHandling::isCleanupRequested()) {
            throw std::runtime_error("Elevation player killed by signal.");
        }
    }
}

void ElevationPlayerEventManager::connectionThread_(std::shared_ptr<Mp3EventSocket> eventSocket, Logger& logger) {
    logger.log("New connection accepted");

    ElevationPlayerMp3EventVisitor visitor{logger, eventSocket, m_autoPlayer};
    while (true) {
        try {
            eventSocket->readEvent()->acceptVisitor(visitor);
        }
        catch (const std::exception& e) {
            logger.err(std::string("ElevationPlayerEventManager got C++ exception: ") + e.what());
        }
    }
}

} // namespace elevation
