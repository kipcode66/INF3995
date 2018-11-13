#include <iostream>
#include <algorithm>
#include <future>

#include <common/logger/Logger.hpp>
#include <filesystem/FileCache.hpp>

#include "ArgsParser.hpp"
#include "mp3/Mp3AutoPlayerCallbacks.hpp"

void waitForever() {
    std::promise<void>().get_future().wait();
}

int main(int argc, char** argv) {
    using namespace elevation;

    std::vector<std::string> args;
    std::transform(
        argv,
        argv + argc,
        std::back_inserter(args),
        [](const char* arg) { return std::string(arg); }
    );
    ArgsParser argsParser{args};
    FileCache fileCache{argsParser.getCachePath()};
    Mp3AutoPlayerCallbacks autoPlayer{Logger::getLogger("elevation-player"), fileCache};
    waitForever();
    return 0;
}
