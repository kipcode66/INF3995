#ifndef EVENT_EVENTMANAGER_HPP
#define EVENT_EVENTMANAGER_HPP

#include <thread>

#include <common/mp3/communication/Mp3EventListenerSocket.hpp>
#include <common/mp3/communication/Mp3EventSocket.hpp>

namespace elevation {

class EventManager {
public:
    explicit EventManager(uint16_t port);
    virtual ~EventManager();

protected:
    static void accepterThread_(Mp3EventListenerSocket listener);
    static void connectionThread_(std::unique_ptr<Mp3EventSocket> socket);

protected:
    std::thread m_accepterThread;
};

} // namespace elevation

#endif // !EVENT_EVENTMANAGER_HPP
