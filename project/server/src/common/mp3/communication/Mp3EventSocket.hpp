#ifndef MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP
#define MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP

#include "os/Socket.hpp"
#include "mp3/event/Mp3Event.hpp"

namespace elevation {

class Mp3EventSocket : public Socket {
public:
    /**
     * @brief Makes an Mp3EventSocket from an accepted Socket.
     * The latter may not be used again after this call.
     */
    explicit Mp3EventSocket(Socket&& socket);
    Mp3EventSocket(const Mp3EventSocket&) = delete;
    Mp3EventSocket(Mp3EventSocket&& that);
    virtual ~Mp3EventSocket();

    Mp3EventSocket& operator=(const Mp3EventSocket&) = delete;
    Mp3EventSocket& operator=(Mp3EventSocket&& that);
    
    void write(const Mp3Event& event);
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENTCLIENTSOCKET_HPP
