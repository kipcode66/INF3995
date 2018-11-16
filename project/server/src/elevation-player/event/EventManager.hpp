#ifndef EVENT_EVENTMANAGER_HPP
#define EVENT_EVENTMANAGER_HPP

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>

namespace elevation {

class EventManager {
public:
    explicit EventManager(uint16_t port);
    virtual ~EventManager();

protected:
    Mp3EventListenerSocket m_listener;
};

} // namespace elevation

#endif // !EVENT_EVENTMANAGER_HPP
