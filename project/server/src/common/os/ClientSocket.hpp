#ifndef OS_CLIENT_SOCKET_HPP
#define OS_CLIENT_SOCKET_HPP

#include "Socket.hpp"

namespace elevation {

class ClientSocket : public virtual Socket {
public:
    ClientSocket(uint16_t portNum);
    ClientSocket(const ClientSocket&) = delete;
    ClientSocket(ClientSocket&& that);

    ~ClientSocket();

    ClientSocket& operator=(const ClientSocket&) = delete;
    ClientSocket& operator=(ClientSocket&& that);
};

} // namespace elevation

#endif // !OS_CLIENT_SOCKET_HPP
