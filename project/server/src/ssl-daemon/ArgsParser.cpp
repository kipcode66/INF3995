#include "ArgsParser.hpp"

namespace elevation {
namespace daemon {

ArgsParser::ArgsParser() { }

ArgsParser::Config ArgsParser::parseArgs(int argc, char** argv) {
    return Config(0, 0);
}

} // namespace daemon
} // namespace elevation
