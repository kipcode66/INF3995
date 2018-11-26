#include "Mp3EventClientSocket.hpp"

namespace elevation {

Mp3EventClientSocket::Mp3EventClientSocket(uint16_t port)
    : Socket(port)
    , ClientSocket(port)
    , Mp3EventSocket() // Call to protected constructor of Mp3EventSocket
{ }

Mp3EventClientSocket::Mp3EventClientSocket(Mp3EventClientSocket&& that)
    : Socket(std::move(that))
    , ClientSocket(std::move(that))
    , Mp3EventSocket(std::move(that))
{ }

Mp3EventClientSocket::~Mp3EventClientSocket() { }

Mp3EventClientSocket& Mp3EventClientSocket::operator=(Mp3EventClientSocket&& that) {
    static_cast<ClientSocket&>(*this) = std::move(that);
    return *this;
}

} // namespace elevation
