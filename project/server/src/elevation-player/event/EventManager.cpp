#include "EventManager.hpp"

namespace elevation {

EventManager::EventManager(uint16_t port)
    : m_listener(port)
{

}

EventManager::~EventManager() { }

} // namespace elevation