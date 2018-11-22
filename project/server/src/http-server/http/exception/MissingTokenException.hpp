#ifndef HTTP_EXCEPTION_MISSINGTOKENEXCEPTION_HPP
#define HTTP_EXCEPTION_MISSINGTOKENEXCEPTION_HPP

#include <stdexcept>

#include "BadRequestException.hpp"

namespace elevation {

class MissingTokenException : public BadRequestException {
public:
    virtual const char* what() const noexcept override;
};

} // namespace elevation

#endif // !HTTP_EXCEPTION_MISSINGTOKENEXCEPTION_HPP
