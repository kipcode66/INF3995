#ifndef COMMUNICATION_LISTENER_SOCKET_HPP
#define COMMUNICATION_LISTENER_SOCKET_HPP

#include "IpSocket.hpp"

namespace elevation {
namespace daemon {

class ListenerSocket : public IpSocket {
public:
    static constexpr int SOCKET_MAX_BACKLOG = 10; ///< @brief Max Number of pending connections before we drop the next ones.

public:
    ListenerSocket(uint16_t portNum);
    ListenerSocket(const ListenerSocket&) = delete;
    ListenerSocket(ListenerSocket&&);
    virtual ~ListenerSocket();

    ListenerSocket& operator=(const ListenerSocket&) = delete;
    ListenerSocket& operator=(ListenerSocket&&);

    IpSocket accept();
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_LISTENER_SOCKET_HPP
