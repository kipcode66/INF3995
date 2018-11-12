#include "RestServerArgsParser.hpp"

#include <iostream>
#include <sstream>

namespace elevation {

RestServerArgsParser::RestServerArgsParser(const std::vector<std::string>& args) {
    static constexpr int NUM_REQUIRED_ARGS = 2;
    if (args.size() < NUM_REQUIRED_ARGS) {
        usage_();
        exit(1);
    }

    for (std::size_t i = 1; i < args.size(); ++i) {
        const std::string arg = args.at(i);
        if (arg == "-c" || arg == "--cache-path") {
            try {
                m_cachePath = args.at(i + 1);
                ++i;
            } catch (std::out_of_range& e) {
                throw std::invalid_argument("Missing argument after -c.");
            }
        }
        else if (arg == "-p" || arg == "--port") {
            try {
                m_port = parsePort_(args.at(i + 1));
                ++i;
            } catch (std::out_of_range& e) {
                throw std::invalid_argument("Missing argument after -p.");
            }
        }
        else if (arg == "-h" || arg == "--help") {
            usage_();
            exit(0);
        }
        else {
            throw std::invalid_argument("Unknown argument \"" + args.at(i) + "\"");
        }
    }
}

uint16_t RestServerArgsParser::parsePort_(const std::string& portString) const {
    uint32_t portId;
    std::istringstream iStrStrm(portString);
    iStrStrm >> portId;

    if (portId == 0 || portId > UINT32_MAX) {
        throw std::invalid_argument("Cannot bind server to port \"" + portString + "\"");
    }
    else {
        return portId;
    }
}

void RestServerArgsParser::usage_() const {
    std::cerr <<
        "USAGE:\n"
        "    http-server --help | -h\n"
        "    http-server <PORT_OPTION> <CACHE_PATH_OPTION>\n"
        "\n"
        "OPTIONS:\n"
        "    -h | --help               Print this message and exit.\n"
        "    -p | --port <PORT>        Port to listen on.\n"
        "    -c | --cache-path <PATH>  Path to the cache directory, where the MP3 songs are.";
}

} // namespace elevation
