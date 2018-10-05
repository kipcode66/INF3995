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
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int ipSocketFd = ::accept(m_fd, (struct sockaddr*)&addr, &len);
    if (ipSocketFd < 0) {
        throw std::runtime_error(::strerror(errno));
    }
    IpSocket socketObject(m_portNum, ipSocketFd);
    return socketObject;
}

} // namespace daemon
} // namespace elevation
