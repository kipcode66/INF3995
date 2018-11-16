#include "EventManager.hpp"

#include <functional>

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>

namespace elevation {

EventManager::EventManager(uint16_t port)
{
    Mp3EventListenerSocket listener{port};
    m_accepterThread = std::thread(std::bind(&EventManager::accepterThread_, std::move(listener)));
}

EventManager::~EventManager() {
    // NOTE: In theory, since the Elevation Player never finishes until we kill it,
    // This destructor should never get called. However, if it is, it will block forever.
    m_accepterThread.join();
}

void EventManager::accepterThread_(Mp3EventListenerSocket listener) {
    // TODO
}

void EventManager::connectionThread_() {
    // TODO
}

} // namespace elevation