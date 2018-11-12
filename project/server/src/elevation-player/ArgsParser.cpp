#include "ArgsParser.hpp"

#include <iostream>

namespace elevation {

ArgsParser::ArgsParser(const std::vector<std::string>& args) {
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
                throw std::invalid_argument("Missing argument.");
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

void ArgsParser::usage_() {
    std::cerr <<
        "USAGE:\n"
        "    elevation-player <OPTION>\n"
        "\n"
        "OPTIONS:\n"
        "    -c | --cache-path <PATH>   Path to the cache directory, where the MP3 songs are.\n"
        "    -h | --help                Show this help message and exit\n";
}

} // namespace elevation
