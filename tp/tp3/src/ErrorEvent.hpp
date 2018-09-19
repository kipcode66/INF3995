#ifndef SRC_ERROREVENT_HPP
#define SRC_ERROREVENT_HPP

#include "LogEvent.hpp"

class ErrorEvent: public LogEvent {
public:

    ErrorEvent(std::ostream& out, const Pistache::Http::Code& errorCode, const std::string& resource);
    ~ErrorEvent();
    virtual void print();

private: 
    std::string m_resource;
    Pistache::Http::Code m_errorCode;
};

#endif // !SRC_ERROREVENT_HPP
 