#ifndef COMMUNICATION_IP_SOCKET_HPP
#define COMMUNICATION_IP_SOCKET_HPP

#include "Socket.hpp"

#include <inttypes.h>

namespace elevation {
namespace daemon {
    
class IpSocket : public Socket {
public:
    explicit IpSocket(uint16_t portNum);
    virtual ~IpSocket();

protected:
    uint16_t m_portNum;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_IP_SOCKET_HPP