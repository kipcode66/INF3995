#include <iostream>
#include <algorithm>

#include "ArgsParser.hpp"

int main(int argc, char** argv) {
    std::vector<std::string> args;
    std::transform(
        argv,
        argv + argc,
        std::back_inserter(args),
        [](const char* arg) { return std::string(arg); }
    );
    elevation::ArgsParser argsParser{args};
    return 0;
}
