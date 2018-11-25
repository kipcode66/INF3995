#ifndef HTTP_EXCEPTION_INVALIDTOKENEXCEPTION_HPP
#define HTTP_EXCEPTION_INVALIDTOKENEXCEPTION_HPP

#include <stdexcept>
#include <inttypes.h>
#include <memory>

namespace elevation {

class InvalidTokenException : public std::exception {
protected:
    static std::shared_ptr<char[]> makeExceptionMessage_(std::string receivedToken);

public:
    explicit InvalidTokenException(std::string receivedToken);
    virtual const char* what() const noexcept override;

protected:
    std::shared_ptr<char[]> m_errorMessage;
};

} // namespace elevation

#endif // !HTTP_EXCEPTION_INVALIDTOKENEXCEPTION_HPP
