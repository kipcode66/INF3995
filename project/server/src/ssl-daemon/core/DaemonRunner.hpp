#ifndef CORE_DAEMON_RUNNER_HPP
#define CORE_DAEMON_RUNNER_HPP

#include "ssl/SslSession.hpp"

#include <thread>

namespace elevation {
namespace daemon {

class DaemonRunner {
public:
    DaemonRunner(SslSession session);
    ~DaemonRunner();

protected:
    void runner_(SslSession clientSession);
    void reader_(SslSession& clientSession);
    void writer_(SslSession& clientSession);
};

} // namespace daemon
} // namespace elevation

#endif // !CORE_DAEMON_RUNNER_HPP
