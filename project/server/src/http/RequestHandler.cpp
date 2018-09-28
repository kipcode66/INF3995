#include "RequestHandler.hpp"

namespace elevation {
namespace http {

RequestHandler::RequestHandler()
{ }

RequestHandler::~RequestHandler()
{ }

void RequestHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Not_Found, "404 not found");
}

} // namespace http
} // namespace elevation
