#include "LocalSocket.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>

#include <elevation/common.hpp>

namespace elevation {
namespace daemon {

LocalSocket::LocalSocket(const std::string& socketFilename)
{
    // Create socket
    int socket_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    // Bind to address and port
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    std::string socketPath = std::string(elevation::common::SOCKET_DIR) + "/" + socketFilename;

    if (socketPath.size() >= sizeof(addr.sun_path)) {
        throw std::runtime_error("Path \"" + socketPath + "\" too long to bind to a socket.");
    }

    ::strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path));
    if (::bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    m_fd = socket_fd;
    m_socketPath = socketPath;
    std::cout << "Local Socket created at fd " << m_fd << std::endl;
}

LocalSocket::~LocalSocket() {
    if (m_fd != NO_FD) {
        ::close(m_fd);
        ::remove(m_socketPath.c_str());
    }
}

} // namespace daemon
} // namespace elevation
