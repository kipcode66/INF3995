#include "ArgsParser.hpp"

#include <iostream>
#include <stdexcept>

#include "core/DaemonMaster.hpp"

#include <common/os/signal/SignalHandling.hpp>

int main(int argc, char** argv) {
    using namespace elevation;
    using namespace elevation::daemon;

    SignalHandling::installSignalHandlers();

    ArgsParser argsParser;
    ArgsParser::Config config(0, 0);
    try {
        config = argsParser.parseArgs(std::vector<std::string>(argv + 1, argv + argc));
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        DaemonMaster runner(config);
        std::cout << "SSL Daemon starting." << std::endl;
        runner.run();
        std::cout << "SSL Daemon says goodbye!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "SSL Daemon stopped after catching C++ exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "SSL Daemon stopped after catching unknown exception." << std::endl;
        return 1;
    }
    return 0;
}
