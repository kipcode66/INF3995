#include "Mp3EventClientSocket.hpp"

namespace elevation {

Mp3EventClientSocket::Mp3EventClientSocket(uint16_t port)
    : ClientSocket(port)
{ }

Mp3EventClientSocket::Mp3EventClientSocket(Mp3EventClientSocket&& that)
    : ClientSocket(std::move(that))
{ }

Mp3EventClientSocket::~Mp3EventClientSocket() { }

Mp3EventClientSocket& Mp3EventClientSocket::operator=(Mp3EventClientSocket&& that) {
    static_cast<ClientSocket&>(*this) = std::move(that);
    return *this;
}

void Mp3EventClientSocket::write(const Mp3Event& event) {
    std::string serializedEvent = event.serialize();
    Socket::writeRaw(std::move(serializedEvent));
}

} // namespace elevation
