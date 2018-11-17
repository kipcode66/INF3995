#include "Mp3EventSocket.hpp"

namespace elevation {

Mp3EventSocket::Mp3EventSocket(Socket&& socket)
    : Socket(std::move(socket))
    , m_packetReader(*this)
{ }

Mp3EventSocket::Mp3EventSocket(Mp3EventSocket&& that)
    : Socket(std::move(that))
    , m_packetReader(*this)
{ }

Mp3EventSocket::~Mp3EventSocket() { }

Mp3EventSocket& Mp3EventSocket::operator=(Mp3EventSocket&& that) {
    static_cast<Socket&>(*this) = std::move(that);
    return *this;
}

std::unique_ptr<Mp3Event> Mp3EventSocket::readEvent() {
    return Mp3EventPacketReader{*this}.readEvent();
}

} // namespace elevation
