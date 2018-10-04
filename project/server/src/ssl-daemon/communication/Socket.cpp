#include "Socket.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

namespace elevation {
namespace daemon {
    
Socket::Socket(uint16_t portNum, bool isLocalhostSocket)
{
    m_fd = NO_FD;

    if (isLocalhostSocket) {
        createLocalSocket_(portNum);
    }
    else {
        createIpSocket_(portNum);
    }
}

Socket::~Socket()
{
    if (m_fd != NO_FD) {
        ::close(m_fd);
    }
}

Socket& Socket::operator<<(const std::string& str) {
    return *this;
}

Socket& Socket::operator>>(std::string& str) {
    return *this;
}

void Socket::createLocalSocket_(uint16_t portNum) {
}

void Socket::createIpSocket_(uint16_t portNum) {
}

} // namespace daemon
} // namespace elevation
