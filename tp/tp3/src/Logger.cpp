#include "Logger.hpp"

Logger::Logger(std::ostream& out)
    : m_out(out)
{ }

Logger::~Logger()
{ }

void Logger::logRequest(Pistache::Http::Method method, const std::string& resource) {
    m_out << "Got \"" << method << "\" request for resource \"" << resource << '"' << std::endl;
}

void Logger::logError(Pistache::Http::Method method, Pistache::Http::Code errorCode, const std::string& resource) {
    m_out << "Got error \"" << errorCode << "\" during request for resource \"" << resource << '"' << std::endl;
}
