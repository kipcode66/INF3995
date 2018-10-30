#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include <string>
#include <fstream>

namespace elevation {

class Logger {
protected:
    static constexpr const char* FILENAME_EXTENSION = "log";
    static constexpr const char* TIME_FORMAT = "%F-%H%M%S";

protected:
    static std::string getFileName_(const std::string& prefix);
    static std::string getTime_();

public:
    explicit Logger(const std::string& prefix);
    virtual ~Logger();

protected:
    std::string m_prefix;
    std::ofstream m_logFile;
};

} // namespace elevation

#endif  // !LOGGER_LOGGER_HPP
