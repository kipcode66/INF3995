#include "ListenerSocket.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

namespace elevation {
namespace daemon {

ListenerSocket::ListenerSocket(uint16_t portNum)
    : IpSocket(portNum)
{
    if (::listen(m_fd, SOCKET_MAX_BACKLOG) < 0) {
        throw std::runtime_error(::strerror(errno));
    }
}

ListenerSocket::~ListenerSocket()
{ }

IpSocket ListenerSocket::accept() {
    // TODO
}

} // namespace daemon
} // namespace elevation
