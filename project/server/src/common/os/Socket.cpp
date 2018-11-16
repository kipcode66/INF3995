#include "Socket.hpp"

#include <unistd.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <vector>

#include "exception/SocketClosedException.hpp"

namespace elevation {

Socket::Socket(uint16_t portNum)
    : m_fd(s_NO_FD)
    , m_portNum(portNum)
{
    // Create socket
    int socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error(::strerror(errno));
    }
    m_fd = socket_fd;
}

Socket::Socket(uint16_t portNum, int fd)
    : m_fd(fd)
    , m_portNum(portNum)
{ }

Socket::Socket(Socket&& that)
    : m_fd(that.m_fd)
    , m_portNum(that.m_portNum)
{
    that.m_fd = s_NO_FD;
}

Socket::~Socket()
{
    if (m_fd != s_NO_FD) {
        ::close(m_fd);
    }
}

Socket& Socket::operator=(Socket&& that) {
    if (m_fd != s_NO_FD) {
        ::close(m_fd);
    }

    m_fd = that.m_fd;
    that.m_fd = s_NO_FD;
    return *this;
}

void Socket::write(const std::string& str) {
    ::write(m_fd, str.c_str(), str.size() + 1);
}

std::string Socket::readLine() {
    constexpr const char LINE_DELIMITER = '\n'; // The server normally sends \r\n, but only the \n interests us.
    std::ostringstream dataStream;

    char nextCharacter = readCharacter_();
    while (nextCharacter != LINE_DELIMITER) {
        dataStream << nextCharacter;
        nextCharacter = readCharacter_();
    }
    dataStream << nextCharacter;

    return dataStream.str();
}

std::string Socket::read(std::size_t dataSize) {
    std::vector<char> data(dataSize + 1);
    int readAmount = ::read(m_fd, &data[0], dataSize);
    if (readAmount < 0) {
        throw std::runtime_error(::strerror(errno));
    }
    else if (readAmount == 0) {
        throw SocketClosedException();
    }
    data[readAmount] = '\0';
    return std::string(&data[0]);
}

char Socket::readCharacter_() {
    char nextCharacter;
    int readAmount = ::read(m_fd, &nextCharacter, sizeof(nextCharacter));
    if (readAmount < 0) {
        throw std::runtime_error(::strerror(errno));
    }
    else if (readAmount == 0) {
        throw SocketClosedException();
    }
    return nextCharacter;
}

} // namespace elevation
