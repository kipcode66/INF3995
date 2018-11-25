#include "InvalidTokenException.hpp"

#include <sstream>
#include <cstring>

namespace elevation {

std::shared_ptr<char[]> InvalidTokenException::makeExceptionMessage_(std::string receivedToken) {
    std::ostringstream error;
    error << "Received invalid token \"" << receivedToken << '"';
    std::string errorString = error.str();
    std::shared_ptr<char[]> errorMessage{new char[errorString.size() + 1]};
    std::strncpy(errorMessage.get(), errorString.c_str(), errorString.size());
    errorMessage[errorString.size()] = '\0';
    return errorMessage;
}

InvalidTokenException::InvalidTokenException(std::string receivedToken)
    : m_errorMessage(makeExceptionMessage_(receivedToken))
{ }

const char* InvalidTokenException::what() const noexcept {
    return m_errorMessage.get();
}

} // namespace elevation
