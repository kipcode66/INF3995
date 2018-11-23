#include <iostream>
#include <inttypes.h>

#include <common/args/RestServerArgsParser.hpp>
#include "http/RestApi.hpp"

#include <common/mp3/event/VolumeChangeEvent.hpp>
#include <chrono>

int main(int argc, char** argv) {
    std::vector<std::string> args;
    std::transform(
        argv,
        argv + argc,
        std::back_inserter(args),
        [](const char* arg) { return std::string(arg); }
    );
    elevation::RestServerArgsParser argsParser{args};

    try {
        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(argsParser.getPort()));
        elevation::Logger& logger = elevation::Logger::getLogger("http-server");
        elevation::FileCache cache{argsParser.getCachePath()};

        elevation::RestApi api(addr, logger, cache);
        api.init();
        logger.log("Server is about to start.");
        api.start();
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
