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

    // Bind to address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
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

IpSocket::~IpSocket() { }

} // namespace daemon
} // namespace elevation