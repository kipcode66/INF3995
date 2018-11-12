#include "RestServerArgsParser.hpp"
#include <iostream>

namespace elevation {

RestServerArgsParser::RestServerArgsParser(const std::vector<std::string>& args) {

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
