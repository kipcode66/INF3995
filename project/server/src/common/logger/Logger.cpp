#include "Logger.hpp"

#include <elevation/common/Config.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>

namespace elevation {

Logger::Logger(const std::string& prefix)
    : m_prefix(prefix)
    , m_logFile(getFileName_(prefix), std::ios_base::trunc)
{ }

Logger::~Logger() { }

std::string Logger::getFileName_(const std::string& prefix) {
    std::ostringstream osStream;
    osStream << SERVER_LOG_DIR << '/' << prefix << '-' << getTime_() << '.' << FILENAME_EXTENSION;
    return osStream.str();
}

std::string Logger::getTime_() {
    std::time_t now = std::time(nullptr);
    std::ostringstream osStream;
    osStream << std::put_time<char>(std::localtime(&now), TIME_FORMAT);
    return osStream.str();
}

} // namespace elevation
