#include "EventManager.hpp"

#include <functional>

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>

namespace elevation {

EventManager::EventManager(uint16_t port)
{
    Mp3EventListenerSocket listener{port};
    m_accepterThread = std::thread(std::bind(&EventManager::accepterThread_, std::move(listener)));
}

EventManager::~EventManager() { }

void EventManager::accepterThread_(Mp3EventListenerSocket listener) {
    // TODO
}

void EventManager::connectionThread_() {
    // TODO
}

} // namespace elevation