#include "ClientSocket.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

namespace elevation {

ClientSocket::ClientSocket(uint16_t portNum)
    : Socket(portNum)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(portNum);
    if (::connect(m_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(::strerror(errno));
    }
}

ClientSocket::ClientSocket(ClientSocket&& that)
    : Socket(std::move(that))
{
    m_portNum = that.m_portNum;
}

ClientSocket::~ClientSocket() { }

ClientSocket& ClientSocket::operator=(ClientSocket&& that) {
    if (&that != this) {
        Socket::operator=(std::move(that));
        m_portNum = that.m_portNum;
    }
    return *this;
}

} // namespace elevation
