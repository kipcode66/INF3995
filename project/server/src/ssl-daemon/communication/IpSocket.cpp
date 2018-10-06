#include "IpSocket.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include <elevation/common.hpp>

namespace elevation {
namespace daemon {

IpSocket::IpSocket(uint16_t portNum)
    : m_portNum(portNum)
{
    // Create socket
    int socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error(::strerror(errno));
    }
    m_fd = socket_fd;
}

IpSocket::IpSocket(uint16_t portNum, int fd)
    : m_portNum(portNum)
{
    m_fd = fd;
}

IpSocket::IpSocket(IpSocket&& that)
    : Socket(std::move(that))
    , m_portNum(that.m_portNum)
{ }

IpSocket::~IpSocket() { }

IpSocket& IpSocket::operator=(IpSocket&& that) {
    Socket::operator=(std::move(that));
    m_portNum = that.m_portNum;
    return *this;
}

} // namespace daemon
} // namespace elevation
