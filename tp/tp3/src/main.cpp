#include <iostream>

#include "HelloHandler.hpp"

int main() {
    try {
        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(80));
        
        auto opts = Http::Endpoint::options().threads(1);
        Http::Endpoint server(addr);
        server.init(opts);
        server.setHandler(std::make_shared<HelloHandler>());
        std::cerr << "Server is running" << std::endl;
        server.serve();
        return 0;
    }
    catch (...) {
        std::cerr << "Server crashed with error" << std::endl;
        return -1;
    }
}
