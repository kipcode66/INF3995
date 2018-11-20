#ifndef HTTP_EXCEPTION_MISSINGTOKENEXCEPTION_HPP
#define HTTP_EXCEPTION_MISSINGTOKENEXCEPTION_HPP

#include <stdexcept>

namespace elevation {

class MissingTokenException : public std::exception {
public:
    virtual const char* what() const noexcept override;
};

} // namespace elevation

#endif // !HTTP_EXCEPTION_MISSINGTOKENEXCEPTION_HPP
