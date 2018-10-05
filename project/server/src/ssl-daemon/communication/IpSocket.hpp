#ifndef COMMUNICATION_IP_SOCKET_HPP
#define COMMUNICATION_IP_SOCKET_HPP

#include "Socket.hpp"

#include <inttypes.h>

namespace elevation {
namespace daemon {
    
class IpSocket : public Socket {
public:
    explicit IpSocket(uint16_t portNum); ///< @brief Creates a stream socket at given port.
    explicit IpSocket(uint16_t portNum, int fd); ///< @brief Associates an IpSocket to an existing file descriptor.
    IpSocket(const IpSocket&) = delete;
    IpSocket(IpSocket&&);

    virtual ~IpSocket();

    IpSocket& operator=(const IpSocket&) = delete;
    IpSocket& operator=(IpSocket&&);

protected:
    uint16_t m_portNum;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_IP_SOCKET_HPP