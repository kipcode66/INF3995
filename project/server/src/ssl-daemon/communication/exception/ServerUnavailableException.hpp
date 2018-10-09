#ifndef COMMUNCICATION_EXCEPTIONS_SERVERUNAVAILABLEEXCEPTION_HPP
#define COMMUNCICATION_EXCEPTIONS_SERVERUNAVAILABLEEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace elevation {
namespace daemon {

class ServerUnavailableException : std::exception {
public:
    static const std::string REASON;

public:
    ServerUnavailableException();

    virtual const char* what() const noexcept;
};

} // namespace daemon
} // namespace elevation

#endif // !COMMUNCICATION_EXCEPTIONS_SERVERUNAVAILABLEEXCEPTION_HPP
