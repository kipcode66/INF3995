#include "ArgsParser.hpp"

namespace elevation {
namespace daemon {

ArgsParser::ArgsParser() { }

ArgsParser::Config ArgsParser::parseArgs(const std::vector<std::string>& argv) {
    return Config(0, 0);
}

} // namespace daemon
} // namespace elevation
