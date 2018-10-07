#include "DaemonManager.hpp"
#include "signal/SignalHandling.hpp"
#include "communication/ListenerSocket.hpp"
#include "communication/ClientSocket.hpp"
#include "ssl/SslContext.hpp"
#include "core/DaemonRunner.hpp"

namespace elevation {
namespace daemon {

DaemonManager::DaemonManager(const ArgsParser::Config& config)
    : m_config(config)
{ }

void DaemonManager::run() {
    SslContext::createInstance(m_config.getListenPort());
    SslContext& ctx = SslContext::getInstance();

    while (true) {
        SslSession session = ctx.acceptSession();
        std::cout << "Accepted HTTPS connection." << std::endl;
        ClientSocket httpServerConnection(m_config.getOutputPort());

        DaemonRunner runner(std::move(session));

        if (SignalHandling::cleanupRequested.load()) {
            throw std::runtime_error("SSL Daemon killed by signal.");
        }
    }
}

} // namespace daemon
} // namespace elevation
