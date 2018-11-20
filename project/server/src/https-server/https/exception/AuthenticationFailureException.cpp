#include "AuthenticationFailureException.hpp"

#include <sstream>
#include <cstring>

namespace elevation {

std::shared_ptr<char[]> AuthenticationFailureException::makeTokenFailureMessage_(std::string token) {
    std::ostringstream error;
    error << "Token \"" << token << "\" does not correspond to an admin";
    std::string errorString = error.str();
    std::shared_ptr<char[]> errorMessage{new char[errorString.size() + 1]};
    std::strncpy(errorMessage.get(), errorString.c_str(), errorString.size());
    errorMessage[errorString.size()] = '\0';
    return errorMessage;
}

std::shared_ptr<char[]> AuthenticationFailureException::makeUserPassFailureMessage_(std::string username, std::string password) {
    std::ostringstream error;
    error << "Wrong username/password pair : " << username << " / " << password;
    std::string errorString = error.str();
    std::shared_ptr<char[]> errorMessage{new char[errorString.size() + 1]};
    std::strncpy(errorMessage.get(), errorString.c_str(), errorString.size());
    errorMessage[errorString.size()] = '\0';
    return errorMessage;
}

AuthenticationFailureException::AuthenticationFailureException(std::string token)
    : m_errorMessage(makeTokenFailureMessage_(token))
{ }

AuthenticationFailureException::AuthenticationFailureException(std::string username, std::string password)
    : m_errorMessage(makeUserPassFailureMessage_(username, password))
{ }

const char* AuthenticationFailureException::what() const noexcept {
    return m_errorMessage.get();
}

} // namespace elevation
