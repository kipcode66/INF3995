#ifndef SRC_HTTP_REQUEST_HANDLER_HPP
#define SRC_HTTP_REQUEST_HANDLER_HPP

#include "pistache/endpoint.h"

namespace elevation {
namespace http {

class RequestHandler : public Pistache::Http::Handler {
public:
    HTTP_PROTOTYPE(RequestHandler);

    explicit RequestHandler();
    virtual ~RequestHandler();

    virtual void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
};

}}

#endif // !SRC_HTTP_REQUEST_HANDLER_HPP
