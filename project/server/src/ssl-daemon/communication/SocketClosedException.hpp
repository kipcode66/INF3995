#ifndef COMMUNCICATION_SOCKET_CLOSED_EXCEPTION
#define COMMUNCICATION_SOCKET_CLOSED_EXCEPTION

#include <stdexcept>
#include <string>

namespace elevation {
namespace daemon {

class SocketClosedException : std::exception {
public:
    static const std::string REASON;
public:
    SocketClosedException();

    virtual const char* what() const noexcept;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNCICATION_SOCKET_CLOSED_EXCEPTION
