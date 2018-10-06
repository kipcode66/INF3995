#ifndef COMMUNICATION_LOCAL_SOCKET_HPP
#define COMMUNICATION_LOCAL_SOCKET_HPP

#include "Socket.hpp"

#include <string>

namespace elevation {
namespace daemon {

class LocalSocket : public Socket {
public:
    explicit LocalSocket(const std::string& socketFilename);
    virtual ~LocalSocket();

protected:
    std::string m_socketPath;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNICATION_LOCAL_SOCKET_HPP
