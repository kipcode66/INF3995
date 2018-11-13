#ifndef SRC_REQUESTEVENT_HPP
#define SRC_REQUESTEVENT_HPP

#include "LogEvent.hpp"

class RequestEvent: public LogEvent {
public:

    RequestEvent(std::ostream& out, const Pistache::Http::Method& method, const std::string& resource);
    ~RequestEvent();
    virtual void print();

private:
    std::string m_resource;
    Pistache::Http::Method m_method;
};

#endif // !SRC_REQUESTEVENT_HPP
