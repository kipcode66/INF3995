#ifndef SRC_LOGEVENT_HPP
#define SRC_LOGEVENT_HPP

#include <iostream>
#include <string>
#include <mutex>

#include "pistache/endpoint.h"

class LogEvent {
public:
    explicit LogEvent(std::ostream& out);
    virtual ~LogEvent();

    virtual void print() = 0;

protected:
    std::ostream& m_out;
};

#endif // !SRC_LOGEVENT_HPP
