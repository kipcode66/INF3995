#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include <string>
#include <fstream>
#include <unordered_map>
#include <mutex>
#include <memory>

namespace elevation {

/**
 * @brief This class implements a thread-safe logger.
 */
class Logger {
protected:
    static constexpr const char* FILENAME_EXTENSION = "log";
    static constexpr const char* TIME_FORMAT_FILE_NAME = "%F-%H%M%S";
    static constexpr const char* TIME_FORMAT_LOG_LINE = "%F %H:%M:%S";

protected:
    static std::string getFileName_(const std::string& prefix);
    static std::string getTime_(const char* format);

public:
    static Logger& getLogger(const std::string& prefix);

public:
    explicit Logger(Logger&&) = default;
    virtual ~Logger();

    Logger& operator=(Logger&&) = default;

    void log(const std::string& data);
    void err(const std::string& data);

protected:
    explicit Logger(const std::string& prefix);
    void createDirectory_();
    void openLogFile_();
    std::ostringstream makeLogLine_() const;

protected:
    std::string m_prefix;
    std::ofstream m_logFile;
    std::unique_ptr<std::mutex> m_mutex;

private:
    static std::unordered_map<std::string, Logger> s_loggers;
};

} // namespace elevation

#endif  // !LOGGER_LOGGER_HPP
