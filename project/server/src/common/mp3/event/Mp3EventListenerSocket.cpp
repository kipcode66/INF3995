#include "Mp3EventListenerSocket.hpp"

namespace elevation {

Mp3EventListenerSocket::Mp3EventListenerSocket(uint16_t port)
    : ListenerSocket(port)
{ }

Mp3EventListenerSocket::Mp3EventListenerSocket(Mp3EventListenerSocket&& that)
    : ListenerSocket(std::move(that))
{ }

Mp3EventListenerSocket::~Mp3EventListenerSocket() { }

Mp3EventListenerSocket& Mp3EventListenerSocket::operator=(Mp3EventListenerSocket&& that) {
    static_cast<ListenerSocket&>(*this) = std::move(that);
    return *this;
}

} // namespace elevation
