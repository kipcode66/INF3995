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
    static void runner_(SslSession clientSession, ClientSocket httpServerSocket);
    static void forwardToServer_(std::shared_ptr<SslSession> clientSession, std::shared_ptr<ClientSocket> httpServerSocket, std::shared_ptr<std::shared_future<bool>> tasksReady, std::shared_ptr<std::promise<void>> terminate);
    static void forwardToClient_(std::shared_ptr<SslSession> clientSession, std::shared_ptr<ClientSocket> httpServerSocket, std::shared_ptr<std::shared_future<bool>> tasksReady, std::shared_ptr<std::promise<void>> terminate);

    static void kill_(std::thread& thread);
};

} // namespace daemon
} // namespace elevation

#endif // !CORE_DAEMON_RUNNER_HPP
