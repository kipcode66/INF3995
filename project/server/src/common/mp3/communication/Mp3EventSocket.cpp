#include "Mp3EventSocket.hpp"

namespace elevation {

Mp3EventSocket::Mp3EventSocket(uint16_t port)
    : Socket(port)
{ }

Mp3EventSocket::Mp3EventSocket(uint16_t port, int fd)
    : Socket(port, fd)
{ }

Mp3EventSocket::Mp3EventSocket(Mp3EventSocket&& that)
    : Socket(std::move(that))
{ }

Mp3EventSocket::~Mp3EventSocket() { }

Mp3EventSocket& Mp3EventSocket::operator=(Mp3EventSocket&& that) {
    static_cast<Socket&>(*this) = std::move(that);
    return *this;
}

} // namespace elevation
