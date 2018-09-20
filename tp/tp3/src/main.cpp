#include <iostream>
#include <stdexcept>
#include <inttypes.h>
#include <sstream>
#include <thread>

#include "PrintHandler.hpp"
#include "HelloHandler.hpp"

void usage() {
    std::cerr << std::endl << "Usage: tp3 [portNum]" << std::endl <<
        std::endl <<
        "    portNum: Port to listen on. Default: 80" << std::endl <<
        std::endl <<
        "    NOTE: On Unix systems, opening ports below 1024 requires superuser privileges." << std::endl;
}

int main(int argc, char** argv) {
    uint32_t portId;
    if (argc == 1) {
        portId = 80;
    }
    else if (argc == 2 && std::string(argv[1]) != "-h" && std::string(argv[1]) != "--help") {
        std::istringstream iStrStrm(argv[1]);
        iStrStrm >> portId;
    }
    else {
        usage();
        return 0;
    }

    if (portId == 0 || portId > UINT16_MAX) {
        std::cerr << std::endl <<
            "Cannot bind server to port \"" << argv[1] << "\"" << std::endl;
        usage();
        return -1;
    }

    try {
        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(portId));
        
        auto opts = Http::Endpoint::options().threads(1);
        Http::Endpoint server(addr);
        server.init(opts);
        auto helloHanldlerPointer = std::make_shared<HelloHandler>(); 
        server.setHandler(helloHanldlerPointer);
        std::cerr << "Server is running" << std::endl;
        PrintHandler printHandler = PrintHandler(helloHanldlerPointer->getQueue());
        std::thread printThread(&PrintHandler::print, std::ref(printHandler));
        printThread.detach();
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
