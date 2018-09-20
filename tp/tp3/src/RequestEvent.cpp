#include "RequestEvent.hpp"

RequestEvent::RequestEvent(std::ostream& out, const Pistache::Http::Method& method, const std::string& resource)
    : LogEvent(out), m_method(method), m_resource(resource) 
{ }

RequestEvent::~RequestEvent()
{ }

void RequestEvent::print() {
    std::cerr << "Got \"" << m_method << "\" request for resource \"" << m_resource << '"' << std::endl;
    m_out << "Got \"" << m_method << "\" request for resource \"" << m_resource << '"' << std::endl;
}
