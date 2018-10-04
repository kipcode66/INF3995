#ifndef COMMUNICATION_SOCKET_HPP
#define COMMUNICATION_SOCKET_HPP

#include <iostream>
#include <string>

namespace elevation {
namespace daemon {

class Socket {
public:
    static constexpr int NO_FD = -1;
    static constexpr int SOCKET_MAX_BACKLOG = 100;

public:
    Socket(uint16_t portNum, bool isLocalhostSocket);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) = delete;

    virtual ~Socket();

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) = delete;

    Socket& operator<<(const std::string& str);
    Socket& operator>>(std::string& str);

protected:
    void makeIpSocket_(uint16_t portNum);

protected:
    int m_fd;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_SOCKET_HPP
