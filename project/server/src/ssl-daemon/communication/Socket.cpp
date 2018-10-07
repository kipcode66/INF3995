#include "Socket.hpp"

#include <unistd.h>
#include <sstream>
#include <cstring>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <vector>

namespace elevation {
namespace daemon {

Socket::Socket()
    : m_fd(NO_FD)
{ }

Socket::Socket(Socket&& that)
    : m_fd(NO_FD)
{
    m_fd = that.m_fd;
    that.m_fd = NO_FD;
}

Socket::~Socket()
{
    if (m_fd != NO_FD) {
        ::close(m_fd);
    }
}

Socket& Socket::operator=(Socket&& that) {
    m_fd = that.m_fd;
    that.m_fd = NO_FD;
    return *this;
}

Socket& Socket::operator<<(const std::string& str) {
    ::write(m_fd, str.c_str(), str.size() + 1);
    return *this;
}

Socket& Socket::operator>>(std::string& str) {
    constexpr int READ_SIZE = 1024;
    char buffer[READ_SIZE + 1];

    std::ostringstream oStringStream;
    int bytesRead = 0;
    char* currentBuffer = buffer;
    while (bytesRead < READ_SIZE && bytesRead >= 0) {
        bytesRead += ::read(m_fd, buffer, READ_SIZE - bytesRead);
    }

    if (bytesRead >= 0) {
        buffer[bytesRead] = '\0';
        oStringStream << buffer;
        str = oStringStream.str();
    }
    else {
        throw std::runtime_error(::strerror(errno));
    }
    return *this;
}

std::string Socket::readLine() {
    constexpr const char LINE_DELIMITER = '\n'; // The server normally sends \r\n, but only the \n interests us.
    std::ostringstream dataStream;

    char nextCharacter = readCharacter_();
    while (nextCharacter != LINE_DELIMITER) {
        dataStream << nextCharacter;
        std::cout << "Got char " << nextCharacter << std::endl;
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
        throw std::runtime_error("Socket closed");
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
        throw std::runtime_error("Socket closed");
    }
    return nextCharacter;
}

} // namespace daemon
} // namespace elevation
