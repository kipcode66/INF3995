#include "RequestHandler.hpp"

namespace elevation {
namespace http {

RequestHandler::RequestHandler()
{ }

RequestHandler::~RequestHandler()
{ }

void RequestHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {

}

} // namespace http
} // namespace elevation
