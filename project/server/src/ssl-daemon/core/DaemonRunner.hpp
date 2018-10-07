#ifndef CORE_DAEMON_RUNNER_HPP
#define CORE_DAEMON_RUNNER_HPP

#include "ssl/SslSession.hpp"
#include "communication/ClientSocket.hpp"

#include <thread>

namespace elevation {
namespace daemon {

class DaemonRunner {
public:
    DaemonRunner(SslSession clientSession, ClientSocket httpServerSocket);
    ~DaemonRunner();

protected:
    void runner_(SslSession clientSession, ClientSocket httpServerSocket);
    void reader_(SslSession& clientSession, ClientSocket& httpServerSocket);
    void writer_(SslSession& clientSession, ClientSocket& httpServerSocket);
};

} // namespace daemon
} // namespace elevation

#endif // !CORE_DAEMON_RUNNER_HPP
