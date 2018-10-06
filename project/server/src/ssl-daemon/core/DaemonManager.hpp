#ifndef CORE_DAEMON_MANAGER
#define CORE_DAEMON_MANAGER

#include "ArgsParser.hpp"

namespace elevation {
namespace daemon {

class DaemonManager {
public:
    DaemonManager(const ArgsParser::Config& config);
    void run();

protected:
    const ArgsParser::Config m_config;
};

}
}

#endif // !CORE_DAEMON_MANAGER
