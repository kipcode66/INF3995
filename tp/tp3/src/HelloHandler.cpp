#include "HelloHandler.hpp"

#include "pistache/endpoint.h"

void HelloHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    if (request.method() == Pistache::Http::Method::Get) {
        response.send(Pistache::Http::Code::Ok, "Hello, World\n");
    }
    else {
        response.send(Pistache::Http::Code::Not_Found, "404 Not found\n");
    }
}
