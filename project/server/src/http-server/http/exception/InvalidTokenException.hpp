#ifndef HTTP_EXCEPTION_INVALIDTOKENEXCEPTION_HPP
#define HTTP_EXCEPTION_INVALIDTOKENEXCEPTION_HPP

#include <stdexcept>

namespace elevation {

class InvalidTokenException : public std::exception {
public:
    virtual const char* what() const noexcept override;
};

} // namespace elevation

#endif // !HTTP_EXCEPTION_INVALIDTOKENEXCEPTION_HPP
