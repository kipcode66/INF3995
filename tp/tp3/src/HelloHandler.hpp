#ifndef SRC_HELLO_HANDLER_HPP
#define SRC_HELLO_HANDLER_HPP

#include <fstream>

#include "pistache/endpoint.h"
#include "Logger.hpp"

using namespace Pistache;

class HelloHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    explicit HelloHandler();

    void onRequest(const Http::Request& request, Http::ResponseWriter response);

protected:
    void send404(const Pistache::Http::Request& request, Http::ResponseWriter& response);

protected:
    std::ofstream m_logFile;
    Logger m_logger;
};

#endif // !SRC_HELLO_HANDLER_HPP
