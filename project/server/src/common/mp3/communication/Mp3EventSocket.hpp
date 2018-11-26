#ifndef MP3_EVENT_MP3EVENTSOCKET_HPP
#define MP3_EVENT_MP3EVENTSOCKET_HPP

#include <memory>

#include "os/Socket.hpp"
#include "mp3/event/Mp3Event.hpp"
#include "packet/Mp3EventPacketReader.hpp"

namespace elevation {

class Mp3EventSocket : public virtual Socket {
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

    void writeEvent(const Mp3Event& event);
    std::unique_ptr<Mp3Event> readEvent();

protected:
    explicit Mp3EventSocket(); // Use by subclasses to avoid having to move
                               // the Socket base class which was virtual-intialized.

protected:
    Mp3EventPacketReader m_packetReader;
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENTSOCKET_HPP
