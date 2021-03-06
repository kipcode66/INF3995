#include "ArgsParser.hpp"

#include <iostream>

namespace elevation {
namespace daemon {

ArgsParser::ArgsParser() { }

ArgsParser::Config ArgsParser::parseArgs(const std::vector<std::string>& argv) const {
    uint16_t listenPort = s_DEFAULT_LISTEN_PORT;
    uint16_t outputPort = s_DEFAULT_OUTPUT_PORT;

    for (std::size_t i = 0; i < argv.size(); ++i) {
        const std::string arg = argv.at(i);
        if (arg == "-l") {
            try {
                listenPort = std::stoi(argv.at(i + 1));
                ++i;
            } catch (std::out_of_range& e) {
                throw std::invalid_argument("Missing argument after -l");
            }
        }
        else if (arg == "-o") {
            try {
                outputPort = std::stoi(argv.at(i + 1));
                ++i;
            } catch (std::out_of_range& e) {
                throw std::invalid_argument("Missing argument after -o");
            }
        }
        else if (arg == "-h" || arg == "--help") {
            usage();
            exit(0);
        }
        else {
            throw std::invalid_argument("Unknown argument " + argv.at(i));
        }
    }

    return Config(listenPort, outputPort);
}

void ArgsParser::usage() const {
    std::cout <<
        "USAGE" << std::endl <<
        "    ssl-daemon [-l <portNum>] [-o <portNum>]" << std::endl <<
        std::endl <<
        "OPTIONS" << std::endl <<
        "    --help | -h     Print this help menu and exit." << std::endl <<
        "    -l <portNum>    Port number to listen on. [Default: 443]" << std::endl <<
        "    -o <portNum>    Port number to send decyphered output to. [Default: 7777]" << std::endl;

}

} // namespace daemon
} // namespace elevation
