#ifndef CORE_DAEMON_RUNNER_HPP
#define CORE_DAEMON_RUNNER_HPP

#include <thread>
#include <mutex>
#include <future>
#include <vector>
#include <memory>

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
    void forwardToServer_(std::shared_ptr<SslSession> clientSession, std::shared_ptr<ClientSocket> httpServerSocket);
    void forwardToClient_(std::shared_ptr<SslSession> clientSession, std::shared_ptr<ClientSocket> httpServerSocket);
    void killAll_(); ///< Deamon Runners will not hesitate to kill workers if they want to.

protected:
    std::thread m_clientForwarder;
    std::thread m_serverForwarder;
    std::shared_future<bool> m_tasksReady;
};

} // namespace daemon
} // namespace elevation

#endif // !CORE_DAEMON_RUNNER_HPP
