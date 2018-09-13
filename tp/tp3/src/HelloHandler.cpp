#include "HelloHandler.hpp"

#include "pistache/endpoint.h"

void HelloHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    std::cout << "ressource \"" << request.resource() << '"' << std::endl;

    if (request.method() == Pistache::Http::Method::Get) {
        if (request.resource() == "/test1") {
            response.send(Pistache::Http::Code::Ok, "test 1 est bon...\n");
        }
        else if (request.resource() == "/test2") {
            response.send(Pistache::Http::Code::Ok, "test 2 est bon...\n");
        }
        else if (request.resource() == "/test3") {
            response.send(Pistache::Http::Code::Ok, "test 3 est bon...\n");
        }
        else {
            send404(response);
        }
    }
    else {
        send404(response);
    }
}

void HelloHandler::send404(Pistache::Http::ResponseWriter& response) const {
    response.send(Pistache::Http::Code::Not_Found, "404 Not found\n");
}
