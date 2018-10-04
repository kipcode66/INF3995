#include "Socket.hpp"

#include <unistd.h>

namespace elevation {
namespace daemon {
    
Socket::Socket()
    : m_fd(NO_FD)
{ }

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

} // namespace daemon
} // namespace elevation
