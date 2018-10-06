#ifndef COMMUNICATION_CLIENT_SOCKET_HPP
#define COMMUNICATION_CLIENT_SOCKET_HPP

#include "IpSocket.hpp"

namespace elevation {
namespace daemon {

class ClientSocket : public IpSocket {
public:
    ClientSocket(uint16_t portNum);
    ClientSocket(const ClientSocket&) = delete;
    ClientSocket(ClientSocket&& that);

    ~ClientSocket();

    ClientSocket& operator=(const ClientSocket&) = delete;
    ClientSocket& operator=(ClientSocket&& that);
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_CLIENT_SOCKET_HPP
