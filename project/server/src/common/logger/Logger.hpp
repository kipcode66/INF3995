#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include <string>
#include <fstream>

namespace elevation {

class Logger {
public:
    explicit Logger(const std::string& prefix);
    virtual ~Logger();

protected:
    std::string getFileName_() const;
    std::string getTime_() const;

protected:
    std::string m_prefix;
    std::ofstream m_logFile;
};

} // namespace elevation

#endif  // !LOGGER_LOGGER_HPP
