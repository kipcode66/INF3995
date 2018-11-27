#ifndef CORE_DAEMON_RUNNER_HPP
#define CORE_DAEMON_RUNNER_HPP

#include <thread>
#include <mutex>
#include <future>
#include <vector>

#include "ssl/SslSession.hpp"

#include <common/os/ClientSocket.hpp>

namespace elevation {
namespace daemon {

class DaemonRunner {
public:
    DaemonRunner(SslSession clientSession, ClientSocket httpServerSocket);
    ~DaemonRunner();

protected:
    void runner_(SslSession clientSession, ClientSocket httpServerSocket);
    void forwardToServer_(SslSession& clientSession, ClientSocket& httpServerSocket);
    void forwardToClient_(SslSession& clientSession, ClientSocket& httpServerSocket);
    void killAll_(); ///< Deamon Runners will not hesitate to kill workers if they want to.

protected:
    std::vector<std::thread> m_workers;
    std::mutex m_workersMutex;
    std::shared_future<bool> m_tasksReady;
};

} // namespace daemon
} // namespace elevation

#endif // !CORE_DAEMON_RUNNER_HPP
