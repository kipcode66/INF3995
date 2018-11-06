#include "Logger.hpp"

#include <elevation/common/Config.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <experimental/filesystem>

namespace elevation {

std::unordered_map<std::string, Logger> Logger::s_loggers;

Logger::Logger(const std::string& prefix)
    : m_prefix(prefix)
    , m_mutex(new std::mutex())
{
    createDirectory_();
    openLogFile_();
}

Logger::~Logger() {
    bool isEmpty = (m_mutex == nullptr);
    if (!isEmpty) {
        log(m_prefix + " cleanly stopped.");
    }
}

Logger& Logger::getLogger(const std::string& prefix) {
    try {
        return s_loggers.at(prefix);
    }
    catch (const std::out_of_range& e) {
        std::pair<const std::string, Logger> pair(prefix, Logger(prefix));
        s_loggers.insert(std::move(pair));
        return s_loggers.at(prefix);
    }
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
    std::lock_guard<std::mutex> lock(*m_mutex);
    m_logFile << '[' << m_prefix << ' ' << getTime_(TIME_FORMAT_LOG_LINE) << "] " << data << std::endl; // std::endl will flush the file stream ; this is intentional.
}

void Logger::createDirectory_() {
    std::error_code error;
    std::experimental::filesystem::create_directories(SERVER_LOG_DIR, error);
    if ((bool)error) {
        throw std::runtime_error(error.message());
    }
}

void Logger::openLogFile_() {
    m_logFile.open(getFileName_(m_prefix), std::ios_base::trunc);
    log(m_prefix + " logger started.");
}

} // namespace elevation
