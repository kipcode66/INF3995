#ifndef SRC_LOGGER_HPP
#define SRC_LOGGER_HPP

#include <iostream>
#include <string>

#include "pistache/endpoint.h"

class Logger {
public:
    explicit Logger(std::ostream& out);
    virtual ~Logger();

    void logRequest(Pistache::Http::Method method, const std::string& resource);
    void logError(Pistache::Http::Method method, Pistache::Http::Code errorCode, const std::string& resource);

private:
    std::ostream& m_out;
};

#endif // !SRC_LOGGER_HPP
