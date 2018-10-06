#include "ListenerSocket.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

namespace elevation {
namespace daemon {

ListenerSocket::ListenerSocket(uint16_t portNum)
    : IpSocket(portNum)
{
    // Bind socket to address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);

    if (::bind(m_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    // Make this socket wait for connections.
    if (::listen(m_fd, SOCKET_MAX_BACKLOG) < 0) {
        throw std::runtime_error(::strerror(errno));
    }
}

ListenerSocket::ListenerSocket(ListenerSocket&& that)
    : IpSocket(std::move(that))
{
    *this = std::move(that);
}

ListenerSocket::~ListenerSocket()
{ }

ListenerSocket& ListenerSocket::operator=(ListenerSocket&& that) {
    m_portNum = that.m_portNum;
}

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
