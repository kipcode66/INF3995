#ifndef MP3_EVENT_MP3EVENTLISTENERSOCKET_HPP
#define MP3_EVENT_MP3EVENTLISTENERSOCKET_HPP

#include "os/ListenerSocket.hpp"
#include "Mp3EventSocket.hpp"

namespace elevation {

class Mp3EventListenerSocket : public ListenerSocket {
public:
    explicit Mp3EventListenerSocket(uint16_t port);
    Mp3EventListenerSocket(const Mp3EventListenerSocket& that) = delete;
    Mp3EventListenerSocket(Mp3EventListenerSocket&& that);
    virtual ~Mp3EventListenerSocket();

    Mp3EventListenerSocket& operator=(const Mp3EventListenerSocket& that) = delete;
    Mp3EventListenerSocket& operator=(Mp3EventListenerSocket&& that);

    virtual std::unique_ptr<Socket> accept() override;
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENTLISTENERSOCKET_HPP
