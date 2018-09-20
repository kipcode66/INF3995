#include "ErrorEvent.hpp"

ErrorEvent::ErrorEvent(std::ostream& out, const Pistache::Http::Code& errorCode, const std::string& resource)
    : LogEvent(out), m_errorCode(errorCode), m_resource(resource)
{ }

ErrorEvent::~ErrorEvent()
{ }

void ErrorEvent::print() {
    std::cerr << "Got error \"" << m_errorCode << "\" during request for resource \"" << m_resource << '"' << std::endl;
    m_out << "Got error \"" << m_errorCode << "\" during request for resource \"" << m_resource << '"' << std::endl;
}
