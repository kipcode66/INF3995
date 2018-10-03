#include "ArgsParser.hpp"

#include <iostream>

namespace elevation {
namespace daemon {

ArgsParser::ArgsParser() { }

ArgsParser::Config ArgsParser::parseArgs(const std::vector<std::string>& argv) {
    for (const std::string& arg : argv) {
        std::cout << '"' << arg << "\" ";
    }
    std::cout << std::endl;

    return Config(0, 0);
}

} // namespace daemon
} // namespace elevation
