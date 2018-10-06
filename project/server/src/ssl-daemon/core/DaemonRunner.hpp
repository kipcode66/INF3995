#ifndef CORE_DAEMON_RUNNER
#define CORE_DAEMON_RUNNER

#include "ArgsParser.hpp"

namespace elevation {
namespace daemon {

class DaemonRunner {
public:
    DaemonRunner(const ArgsParser::Config& config);
    void run();

protected:
    const ArgsParser::Config m_config;
};

}
}

#endif // !CORE_DAEMON_RUNNER
