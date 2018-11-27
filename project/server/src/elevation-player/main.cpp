#include <iostream>
#include <algorithm>
#include <future>

#include <common/logger/Logger.hpp>
#include <filesystem/FileCache.hpp>

#include "ArgsParser.hpp"
#include "mp3/Mp3AutoPlayerCallbacks.hpp"
#include "mp3/event/ElevationPlayerEventManager.hpp"

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

    Logger& logger = Logger::getLogger("elevation-player");
    Mp3AutoPlayerCallbacks autoPlayer{logger, fileCache};
    ElevationPlayerEventManager eventMgr{argsParser.getPort(), logger, std::move(autoPlayer.getReferenceToAutoPlayer())};
    waitForever();
    return 0;
}
