#include <iostream>
#include <stdexcept>
#include <inttypes.h>
#include <sstream>
#include <thread>

#include "PrintHandler.hpp"
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
        auto helloHanldlerPointer = std::make_shared<HelloHandler>(); 
        server.setHandler(helloHanldlerPointer);
        std::cerr << "Server is running" << std::endl;
        PrintHandler printHandler = PrintHandler(helloHanldlerPointer->getQueue());
        std::thread printThread(&PrintHandler::print, std::ref(printHandler));
        //std::thread printThread = std::thread(&PrintHandler::print, std::ref(printHandler)); 
        server.serve();
        printThread.join();

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
