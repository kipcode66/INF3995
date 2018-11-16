#include "EventManager.hpp"

#include <functional>

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>
#include <common/os/signal/SignalHandling.hpp>

namespace elevation {

EventManager::EventManager(uint16_t port)
{
    Mp3EventListenerSocket listener{port};
    m_accepterThread = std::thread(&EventManager::accepterThread_, std::move(listener));
}

EventManager::~EventManager() {
    // NOTE: In theory, since the Elevation Player never finishes until we kill it,
    // This destructor should never get called. However, if it is, it will block forever.
    m_accepterThread.join();
}

void EventManager::accepterThread_(Mp3EventListenerSocket listener) {
    while (true) {
        try {
            std::unique_ptr<Socket> socket = listener.accept();
            std::thread(&EventManager::connectionThread_, std::move(socket)).detach();
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

void EventManager::connectionThread_(std::unique_ptr<Socket> socket) {
    std::cout << "Connection accepted" << std::endl;
}

} // namespace elevation