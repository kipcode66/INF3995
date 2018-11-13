#ifndef CORE_DAEMON_MASTER
#define CORE_DAEMON_MASTER

#include "ArgsParser.hpp"

namespace elevation {
namespace daemon {

class DaemonMaster {
public:
    DaemonMaster(const ArgsParser::Config& config);
    void run();

protected:
    const ArgsParser::Config m_config;
};

} // namespace daemon
} // namespace elevation

#endif // !CORE_DAEMON_MASTER
