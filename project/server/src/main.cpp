#include <iostream>
#include <inttypes.h>

#include "http/RequestHandler.hpp"

int main() {
    uint16_t portId = 80;

    try {
        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(portId));

        auto opts = Pistache::Http::Endpoint::options().threads(1);
        Pistache::Http::Endpoint server(addr);
        server.init(opts);

        auto requestHandler = std::make_shared<elevation::http::RequestHandler>();
        server.setHandler(requestHandler);
        server.serve();
    }
    catch (std::exception& e) {
        std::cerr << "Server crashed with C++ exception: \"" << e.what() << "\"" << std::endl;
        return 255;
    }
    catch (...) {
        std::cerr << "Server crashed with unknown exception type" << std::endl;
        return 254;
    }
    return 0;
}
