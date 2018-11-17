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
        throw std::runtime_error("Could not read event from socket : recieved data is not an event");
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
        std::cout << "Got volume change event" << std::endl;
        break;
    default:
        throw std::runtime_error("Unknown event type " + std::to_string(static_cast<std::size_t>(eventType)));
        break;
    }
    return nullptr;
}

} // namespace elevation
