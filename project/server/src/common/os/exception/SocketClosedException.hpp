#ifndef OS_EXCEPTIONS_SOCKETCLOSEDEXCEPTION_HPP
#define OS_EXCEPTIONS_SOCKETCLOSEDEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace elevation {

class SocketClosedException : std::exception {
public:
    static const std::string s_REASON;

public:
    SocketClosedException();

    virtual const char* what() const noexcept;
};

} // namespace elevation

#endif // !OS_EXCEPTIONS_SOCKETCLOSEDEXCEPTION_HPP
