#ifndef COMMUNCICATION_EXCEPTIONS_SOCKETCLOSEDEXCEPTION_HPP
#define COMMUNCICATION_EXCEPTIONS_SOCKETCLOSEDEXCEPTION_HPP

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

#endif // !COMMUNCICATION_EXCEPTIONS_SOCKETCLOSEDEXCEPTION_HPP
