#include "HelloHandler.hpp"

#include <regex>

HelloHandler::HelloHandler()
    : m_logFile("server.log", std::ofstream::out | std::ofstream::app), m_printQueue(m_printQueue)
{
    m_logFile << "--- SERVER STARTED ---" << std::endl;
    m_printQueue = new PrintQueue();
}

void HelloHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    std::cout << "ressource \"" << request.resource() << '"' << std::endl;

    RequestEvent* requestEventLog = new RequestEvent(m_logFile, request.method(), request.resource());
    m_printQueue->push(requestEventLog);
    if (request.method() == Pistache::Http::Method::Get) {
        if (std::regex_match(request.resource(), std::regex("/test1/?"))) {
            response.send(Pistache::Http::Code::Ok, "test 1 est bon...\n");
        }
        else if (std::regex_match(request.resource(), std::regex("/test2/?"))) {
            response.send(Pistache::Http::Code::Ok, "<a href=\"http://www.polymtl.ca\">Poly Mtl</a>\n");
        }
        else if (std::regex_match(request.resource(), std::regex("/test3/?"))) {
            response.send(
                Pistache::Http::Code::Ok,
                "<!DOCTYPE html>\n"
                "<html>\n"
                "    <body>\n"
                "        <img "
                "          src=\"https://i.kym-cdn.com/photos/images/original/000/863/220/fbb.jpg\" "
                "          alt=\"This should have been a good joke about Chuck Norris, but Chuck Norris didn't want us to joke about him.\""
                "        />\n"
                "    </body>\n"
                "</html>\n"
            );
        }
        else {
            send404(request, response);
        }
    }
    else {
        send404(request, response);
    }
}

void HelloHandler::send404(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter& response) {
    ErrorEvent* errorEventLog = new ErrorEvent(m_logFile, Pistache::Http::Code::Not_Found, request.resource());
    m_printQueue->push(errorEventLog);
    response.send(
        Pistache::Http::Code::Not_Found,
        "<!DOCTYPE html>\n"
        "<html>\n"
        "    <body>\n"
        "        <div><p>404. Sorry, that's an error.<br/><h2>Sucks to be you</h2></p></div>\n"
        "        <img src=\"https://cdn-images-1.medium.com/max/1600/0*AnVCpSvrAeldg3Rn.\"/>\n"
        "    </body>\n"
        "</html>\n"
    );
}

PrintQueue* HelloHandler::getQueue() {
    return m_printQueue;
}

