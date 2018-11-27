#ifndef MP3_COMMUNICATION_PACKET_MP3EVENTPACKETREADER_HPP
#define MP3_COMMUNICATION_PACKET_MP3EVENTPACKETREADER_HPP

#include <memory>

#include "mp3/event/Mp3Event.hpp"

namespace elevation {

class Mp3EventSocket; // Forward-declaration to break circular dependencies.

/**
 * @brief Class which reads events from a socket.
 * @details This is required because the sockets we read from are blocking.
 * This implies that we should never attempt to read a byte from a socket
 * unless we know we have to read more data before returning the Mp3Event.
 * This class thus 'knows' how events are serialized and how to deserialize
 * them without having to look at any extra byte after the event is deserialized.
 */
class Mp3EventPacketReader {
public:
    explicit Mp3EventPacketReader(Mp3EventSocket& socket);
    virtual ~Mp3EventPacketReader() = default;

    std::unique_ptr<Mp3Event> readEvent();

protected:
    void readSignature_();
    Mp3Event::EventType readType_();
    std::string readPayload_();
    std::unique_ptr<Mp3Event> deserializeEvent_(Mp3Event::EventType, std::string payload);
    std::unique_ptr<Mp3Event> deserializeVolumeChangeEvent_(std::string payload);
    std::unique_ptr<Mp3Event> deserializeMuteEvent_        (std::string payload);
    std::unique_ptr<Mp3Event> deserializeUnmuteEvent_      (std::string payload);
    std::unique_ptr<Mp3Event> deserializeVolumeGetRequest_ (std::string payload);
    std::unique_ptr<Mp3Event> deserializeVolumeGetResponse_(std::string payload);
    std::unique_ptr<Mp3Event> deserializeStopSongEvent_    (std::string payload);

    template <class T>
    static T deserializeElement_(std::string payload, std::size_t offset);

protected:
    Mp3EventSocket& m_socket;
};

} // namespace elevation

#include "mp3/communication/Mp3EventSocket.hpp"

#endif // !MP3_COMMUNICATION_PACKET_MP3EVENTPACKETREADER_HPP
