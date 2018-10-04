#include "Socket.hpp"

#include <sys/socket.h>

namespace elevation {
namespace daemon {
    
Socket::Socket(uint16_t portNum)
{
    m_fd = NO_FD;
}

Socket& Socket::operator<<(const std::string& str) {
    return *this;
}

Socket& Socket::operator>>(std::string& str) {
    return *this;
}

} // namespace daemon
} // namespace elevation
