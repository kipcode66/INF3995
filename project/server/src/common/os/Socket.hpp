#ifndef OS_SOCKET_HPP
#define OS_SOCKET_HPP

#include <iostream>
#include <string>

namespace elevation {

namespace daemon {
class SslSession;
} // namespace daemon

class Socket {
    
    friend class daemon::SslSession; // Requires the file descriptor

public:
    static constexpr int s_NO_FD = -1;

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

    inline uint16_t getPort() const {
        return m_portNum;
    }

protected:
    char readCharacter_();

protected:
    int m_fd;
    uint16_t m_portNum;
};

} // namespace elevation

#endif // !OS_SOCKET_HPP
