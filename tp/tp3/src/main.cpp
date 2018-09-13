#include <iostream>

#include "HelloHandler.hpp"

int main() {
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(80));
    
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serve();
}
