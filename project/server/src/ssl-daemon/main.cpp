#include "ArgsParser.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    elevation::daemon::ArgsParser argsParser;
    try {
        argsParser.parseArgs(std::vector<std::string>(argv + 1, argv + argc));
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 255;
    }

    std::cout << "SSL Daemon says hi!" << std::endl;
    return 0;
}
