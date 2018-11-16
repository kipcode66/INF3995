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

void Mp3EventSocket::write(const Mp3Event& event) {
    Socket::write(event.serialize());
}

std::unique_ptr<Mp3Event> Mp3EventSocket::readEvent() {
    return nullptr;
}

} // namespace elevation
