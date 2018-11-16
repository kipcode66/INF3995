#ifndef MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP
#define MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP

#include "os/Socket.hpp"

namespace elevation {

class Mp3EventSocket : Socket {
public:
    explicit Mp3EventSocket(uint16_t port);
    explicit Mp3EventSocket(uint16_t port, int fd);
    Mp3EventSocket(const Mp3EventSocket&) = delete;
    Mp3EventSocket(Mp3EventSocket&& that);
    virtual ~Mp3EventSocket();

    Mp3EventSocket& operator=(const Mp3EventSocket&) = delete;
    Mp3EventSocket& operator=(Mp3EventSocket&& that);
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP
