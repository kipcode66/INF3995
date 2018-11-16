#include "Mp3EventPacketReader.hpp"

namespace elevation {

Mp3EventPacketReader::Mp3EventPacketReader(Mp3EventSocket& socket)
    : m_socket(socket)
{ }

std::unique_ptr<Mp3Event> Mp3EventPacketReader::readEvent() {
    return nullptr;
}

} // namespace elevation
