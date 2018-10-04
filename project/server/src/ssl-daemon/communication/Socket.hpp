#ifndef COMMUNICATION_SOCKET_HPP
#define COMMUNICATION_SOCKET_HPP

#include <iostream>
#include <string>

namespace elevation {
namespace daemon {

class Socket {
public:
    static constexpr int NO_FD = -1;

public:
    Socket(uint16_t portNum);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) = delete;

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) = delete;

    Socket& operator<<(const std::string& str);
    Socket& operator>>(std::string& str);

private:
    int m_fd;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_SOCKET_HPP
