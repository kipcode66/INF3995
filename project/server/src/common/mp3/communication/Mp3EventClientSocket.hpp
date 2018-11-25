#ifndef MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP
#define MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP

#include "os/ClientSocket.hpp"
#include "mp3/event/Mp3Event.hpp"

namespace elevation {

class Mp3EventClientSocket : public ClientSocket {
public:
    explicit Mp3EventClientSocket(uint16_t port);
    Mp3EventClientSocket(const Mp3EventClientSocket&) = delete;
    Mp3EventClientSocket(Mp3EventClientSocket&& that);
    virtual ~Mp3EventClientSocket();

    Mp3EventClientSocket& operator&(const Mp3EventClientSocket&) = delete;
    Mp3EventClientSocket& operator=(Mp3EventClientSocket&& that);

    void write(const Mp3Event& event);
};

}

#endif // !MP3_EVENT_MP3EVENTSOCKET_HPP
