#ifndef HTTPS_EXCEPTION_AUTHENTICATION_FAILURE_HPP
#define HTTPS_EXCEPTION_AUTHENTICATION_FAILURE_HPP

#include <stdexcept>
#include <string>
#include <memory>

namespace elevation {

class AuthenticationFailureException : public std::exception {
protected:
    static std::shared_ptr<char[]> makeTokenFailureMessage_(std::string token);
    static std::shared_ptr<char[]> makeUserPassFailureMessage_(std::string username, std::string password);

public:
    explicit AuthenticationFailureException(std::string token);
    explicit AuthenticationFailureException(std::string username, std::string password);

    virtual const char* what() const noexcept override;

protected:
    std::shared_ptr<char[]> m_errorMessage;
};

} // HTTPS_EXCEPTION_AUTHENTICATION_FAILURE_HPP

#endif // !HTTPS_EXCEPTION_AUTHENTICATION_FAILURE_HPP
