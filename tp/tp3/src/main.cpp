#include <iostream>
#include <stdexcept>
#include <inttypes.h>
#include <sstream>

#include "HelloHandler.hpp"

int main(int argc, char** argv) {
    uint32_t portId;
    if (argc == 1) {
        portId = 80;
    }
    else {
        std::istringstream iStrStrm(argv[1]);
        iStrStrm >> portId;
    }

    if (portId == 0 || portId > UINT16_MAX) {
        std::cerr << "Cannot bing server to port \"" << portId << "\"";
        return -1;
    }

    try {
        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(portId));
        
        auto opts = Http::Endpoint::options().threads(1);
        Http::Endpoint server(addr);
        server.init(opts);
        server.setHandler(std::make_shared<HelloHandler>());
        std::cerr << "Server is running" << std::endl;
        server.serve();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Server crashed with C++ error: " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "Server crashed with error" << std::endl;
        return -1;
    }
}
