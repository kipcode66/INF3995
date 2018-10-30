#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include <string>

namespace elevation {

class Logger {
public:
    explicit Logger(const std::string& prefix);
    virtual ~Logger();

protected:
    std::string m_prefix;
};

} // namespace elevation

#endif  // !LOGGER_LOGGER_HPP
