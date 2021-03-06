#include "ListenerSocket.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

namespace elevation {

ListenerSocket::ListenerSocket(uint16_t portNum)
    : Socket(portNum)
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
    if (::listen(m_fd, s_SOCKET_MAX_BACKLOG) < 0) {
        throw std::runtime_error(::strerror(errno));
    }
}

ListenerSocket::ListenerSocket(ListenerSocket&& that)
    : Socket(std::move(that))
{ }

ListenerSocket::~ListenerSocket()
{ }

ListenerSocket& ListenerSocket::operator=(ListenerSocket&& that) {
    Socket::operator=(std::move(that));
    return *this;
}

std::unique_ptr<Socket> ListenerSocket::accept() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int ipSocketFd = ::accept(m_fd, (struct sockaddr*)&addr, &len);
    if (ipSocketFd < 0) {
        throw std::runtime_error(::strerror(errno));
    }
    std::unique_ptr<Socket> socketObject{new Socket(m_portNum, ipSocketFd)};
    return socketObject;
}

} // namespace elevation
