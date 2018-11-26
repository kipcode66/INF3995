#include "UnexpectedEventException.hpp"

namespace elevation {

UnexpectedEventException::UnexpectedEventException(const std::string& eventName)
{
    m_eventName = eventName;
}

const char* UnexpectedEventException::what() const noexcept {
    m_errorMessage = std::string{"Unexpected event "} + m_eventName + " occurred";
    return m_errorMessage.c_str();
}

} // namespace elevation
