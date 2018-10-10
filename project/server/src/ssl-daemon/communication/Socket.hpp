#ifndef COMMUNICATION_SOCKET_HPP
#define COMMUNICATION_SOCKET_HPP

#include <iostream>
#include <string>

namespace elevation {
namespace daemon {

class Socket {
    friend class SslSession; // Requires the file descriptor.

public:
    static constexpr int NO_FD = -1;

public:
    Socket(uint16_t portNum);
    Socket(uint16_t portNum, int fd);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other);

    virtual ~Socket();

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other);

    void write(const std::string& str);
    std::string readLine(); ///< Reads data until the next \n character. Throws SocketClosedException if the Socket is closed.
    std::string read(std::size_t dataLength); ///< Reads a certain amount of data. Throws SocketClosedException if the Socket is closed.

    inline int getFd() const {
        return m_fd;
    }

protected:
    char readCharacter_();

protected:
    int m_fd;
    uint16_t m_portNum;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_SOCKET_HPP
