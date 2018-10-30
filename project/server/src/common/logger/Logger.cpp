#include "Logger.hpp"

#include <elevation/common/Config.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <experimental/filesystem>

namespace elevation {

Logger::Logger(const std::string& prefix)
    : m_prefix(prefix)
{
    createDirectory_();
    openLogFile_();
}

Logger::~Logger() {
    log(m_prefix + " cleanly stopped.");
}

std::string Logger::getFileName_(const std::string& prefix) {
    std::ostringstream osStream;
    osStream << SERVER_LOG_DIR << '/' << prefix << '-' << getTime_(TIME_FORMAT_FILE_NAME) << '.' << FILENAME_EXTENSION;
    return osStream.str();
}

std::string Logger::getTime_(const char* format) {
    std::time_t now = std::time(nullptr);
    std::ostringstream osStream;
    osStream << std::put_time<char>(std::localtime(&now), format);
    return osStream.str();
}

void Logger::log(const std::string& data) {
    m_logFile << '[' << m_prefix << ' ' << getTime_(TIME_FORMAT_LOG_LINE) << "] " << data << std::endl; // std::endl will flush the file stream ; this is done on purpose.
}

void Logger::createDirectory_() {
    std::experimental::filesystem::create_directories(SERVER_LOG_DIR);
}

void Logger::openLogFile_() {
    m_logFile.open(getFileName_(m_prefix), std::ios_base::trunc);
    log(m_prefix + " started.");
}

} // namespace elevation
