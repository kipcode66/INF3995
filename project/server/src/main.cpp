#include <iostream>
#include <inttypes.h>

#include "http/RequestHandler.hpp"

int main() {
    uint16_t portId = 80;
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(portId));

    auto opts = Pistache::Http::Endpoint::options().threads(1);
    Pistache::Http::Endpoint server(addr);
    server.init(opts);

    auto requestHandler = std::make_shared<elevation::http::RequestHandler>();
    server.setHandler(requestHandler);
    server.serve();
    return 0;
}
