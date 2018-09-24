#ifndef SRC_HELLO_HANDLER_HPP
#define SRC_HELLO_HANDLER_HPP

#include <fstream>

#include "pistache/endpoint.h"
#include "RequestEvent.hpp"
#include "ErrorEvent.hpp"
#include "PrintHandler.hpp"
#include "PrintQueue.hpp"
#include "thread"

using namespace Pistache;

class HelloHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    explicit HelloHandler(std::ofstream& logFile);

    void onRequest(const Http::Request& request, Http::ResponseWriter response);
    PrintQueue* getQueue();

protected:
    void send404(const Pistache::Http::Request& request, Http::ResponseWriter& response);

protected:
    std::ofstream& m_logFile;

protected:
    static PrintQueue* c_printQueue;
};

#endif // !SRC_HELLO_HANDLER_HPP
