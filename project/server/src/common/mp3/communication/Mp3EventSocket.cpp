#include "Mp3EventSocket.hpp"

namespace elevation {

Mp3EventSocket::Mp3EventSocket(Socket&& socket)
    : Socket(std::move(socket))
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
