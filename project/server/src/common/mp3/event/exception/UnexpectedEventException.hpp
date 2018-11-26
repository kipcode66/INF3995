#ifndef MP3_EVENT_EXCEPTION_UNEXPECTEDEVENTEXCEPTION_HPP
#define MP3_EVENT_EXCEPTION_UNEXPECTEDEVENTEXCEPTION_HPP

#include <stdexcept>

namespace elevation {

class UnexpectedEventException : public std::exception {
public:
    explicit UnexpectedEventException(const std::string& eventName);
    virtual ~UnexpectedEventException() = default;

    const char* what() const noexcept override;

protected:
    std::string m_eventName;
    mutable std::string m_errorMessage; ///< Lazy-initialized within the what() method, which is a const method.
};

} // namespace elevation

#endif // !MP3_EVENT_EXCEPTION_UNEXPECTEDEVENTEXCEPTION_HPP
