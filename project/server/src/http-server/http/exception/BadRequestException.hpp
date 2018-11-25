#ifndef HTTP_EXCEPTION_BADREQUESTEXCEPTION_HPP
#define HTTP_EXCEPTION_BADREQUESTEXCEPTION_HPP

#include <stdexcept>

namespace elevation {

class BadRequestException : public std::exception {
public:
    explicit BadRequestException() = default;
    virtual const char* what() const noexcept override;
};

} // namespace elevation

#endif // !HTTP_EXCEPTION_BADREQUESTEXCEPTION_HPP
