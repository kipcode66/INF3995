#include <iostream>
#include <inttypes.h>

#include "http/RequestHandler.hpp"

void usage() {
    std::cerr << std::endl << "Usage: tp3 [portNum]" << std::endl <<
        std::endl <<
        "    portNum: Port to listen on. Default: 80" << std::endl <<
        std::endl <<
        "    NOTE: On Unix systems, opening ports below 1024 requires superuser privileges." << std::endl;
}

uint16_t parseArgs(int argc, char** argv) {
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
        exit(0);
    }

    if (portId == 0 || portId > UINT16_MAX) {
        std::cerr << std::endl <<
            "Cannot bind server to port \"" << argv[1] << "\"" << std::endl;
        usage();
        exit(254);
    }
}

int main(int argc, char** argv) {

    uint16_t portId = parseArgs(argc, argv);
    constexpr std::size_t MAX_PAYLOAD_SIZE = (100 << 20);

    try {
        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(portId));

        auto opts = Pistache::Http::Endpoint::options().threads(1).maxPayload(MAX_PAYLOAD_SIZE);
        Pistache::Http::Endpoint server(addr);
        server.init(opts);

        auto requestHandler = std::make_shared<elevation::http::RequestHandler>();
        server.setHandler(requestHandler);

        std::cout << "Server is about to start." << std::endl;
        server.serve();
    }
    catch (std::exception& e) {
        std::cerr << "Server crashed with C++ exception: \"" << e.what() << "\"" << std::endl;
        return 255;
    }
    catch (...) {
        std::cerr << "Server crashed with unknown exception type" << std::endl;
        return 255;
    }
    return 0;
}
