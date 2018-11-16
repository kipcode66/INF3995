#ifndef EVENT_EVENTMANAGER_HPP
#define EVENT_EVENTMANAGER_HPP

#include <thread>

namespace elevation {

class EventManager {
public:
    explicit EventManager(uint16_t port);
    virtual ~EventManager();

protected:
    static void accepterThread_(Mp3EventListenerSocket listener);
    static void connectionThread_();

protected:
    std::thread m_accepterThread;
};

} // namespace elevation

#endif // !EVENT_EVENTMANAGER_HPP
