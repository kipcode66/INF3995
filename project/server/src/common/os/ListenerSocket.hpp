#ifndef OS_LISTENER_SOCKET_HPP
#define OS_LISTENER_SOCKET_HPP

#include "Socket.hpp"

namespace elevation {

class ListenerSocket : public Socket {
public:
    static constexpr int s_SOCKET_MAX_BACKLOG = 10; ///< @brief Max Number of pending connections before we drop the next ones.

public:
    ListenerSocket(uint16_t portNum);
    ListenerSocket(const ListenerSocket&) = delete;
    ListenerSocket(ListenerSocket&&);
    virtual ~ListenerSocket();

    ListenerSocket& operator=(const ListenerSocket&) = delete;
    ListenerSocket& operator=(ListenerSocket&&);

    Socket accept();
};

} // namespace elevation

#endif // !OS_LISTENER_SOCKET_HPP
