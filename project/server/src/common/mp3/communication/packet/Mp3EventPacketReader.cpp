#include "Mp3EventPacketReader.hpp"
#include "mp3/event/VolumeChangeEvent.hpp"

namespace elevation {

Mp3EventPacketReader::Mp3EventPacketReader(Mp3EventSocket& socket)
    : m_socket(socket)
{ }

std::unique_ptr<Mp3Event> Mp3EventPacketReader::readEvent() {
    readSignature_();
    Mp3Event::EventType eventType = readType_();
    std::string payload = readPayload_();
    return deserializeEvent_(eventType, std::move(payload));
}

void Mp3EventPacketReader::readSignature_() {
    static const std::string EVENT_SIGNATURE_STRING = Mp3Event::serializeElement(Mp3Event::EVENT_SIGNATURE);
    std::string signature = m_socket.read(EVENT_SIGNATURE_STRING.size());
    if (signature != EVENT_SIGNATURE_STRING) {
        throw std::runtime_error("Could not read event from socket : received data is not an event");
    }
}

Mp3Event::EventType Mp3EventPacketReader::readType_() {
    std::string eventTypeString = m_socket.read(sizeof(Mp3Event::EventType));
    std::istringstream eventTypeStream{eventTypeString};

    Mp3Event::EventType eventType;
    eventTypeStream.read(reinterpret_cast<char*>(&eventType), sizeof(Mp3Event::EventType));
    return eventType;
}

std::string Mp3EventPacketReader::readPayload_() {
    std::string payload = m_socket.read(Mp3Event::PAYLOAD_SIZE);
    return payload;
}

std::unique_ptr<Mp3Event> Mp3EventPacketReader::deserializeEvent_(Mp3Event::EventType eventType, std::string payload) {
    switch(eventType) {
    case Mp3Event::EventType::VOLUME_CHANGE:
        return deserializeVolumeChangeEvent_(std::move(payload));
    default:
        throw std::runtime_error("Unknown event type " + std::to_string(static_cast<std::size_t>(eventType)));
        break;
    }
}

std::unique_ptr<Mp3Event> Mp3EventPacketReader::deserializeVolumeChangeEvent_(std::string payload) {
    auto newVolume = deserializeElement_<uint8_t>(payload, 0);
    return std::unique_ptr<Mp3Event>{new VolumeChangeEvent{newVolume}};
}

template <class T>
T Mp3EventPacketReader::deserializeElement_(std::string payload, std::size_t offset) {
    T element;
    std::istringstream payloadStream{payload};
    payloadStream.seekg(offset);
    payloadStream.read(reinterpret_cast<char*>(&element), sizeof(T));
    return element;
}

} // namespace elevation
