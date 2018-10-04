#include "Socket.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <cstring>

#include <elevation/common.hpp>

namespace elevation {
namespace daemon {
    
Socket::Socket(uint16_t portNum, bool isLocalhostSocket)
{
    m_fd = NO_FD;

    if (isLocalhostSocket) {
        makeLocalSocket_();
    }
    else {
        makeIpSocket_(portNum);
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

void Socket::makeLocalSocket_() {
    // Create socket
    int socket_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    // Bind to address and port
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    std::string socketPath = std::string(elevation::common::SOCKET_DIR);

    if (socketPath.size() >= sizeof(addr.sun_path)) {
        throw std::runtime_error("Path \"" + socketPath + "\" too long to bind to a socket.");
    }

    ::strncpy(addr.sun_path, elevation::common::SOCKET_DIR, sizeof(addr.sun_path));
    if (::bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    m_fd = socket_fd;

    std::cout << "Local Socket created at fd " << m_fd << std::endl;
}

void Socket::makeIpSocket_(uint16_t portNum) {
    // Create socket
    int socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    // Bind to address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    std::string socketPath = std::string(elevation::common::SOCKET_DIR) + "/test.sock";
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);

    if (::bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    if (::listen(socket_fd, SOCKET_MAX_BACKLOG) < 0) {
        throw std::runtime_error(::strerror(errno));
    }
    m_fd = socket_fd;

    std::cout << "Socket created at fd " << m_fd << std::endl;
}

} // namespace daemon
} // namespace elevation
