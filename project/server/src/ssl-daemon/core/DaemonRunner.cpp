#include "DaemonRunner.hpp"
#include "signal/SignalHandling.hpp"
#include "communication/ListenerSocket.hpp"
#include "ssl/SslContext.hpp"

namespace elevation {
namespace daemon {

DaemonRunner::DaemonRunner(const ArgsParser::Config& config)
    : m_config(config)
{ }

void DaemonRunner::run() {
    ListenerSocket listenerSocket(m_config.getListenPort());

    while (true) {
        SslContext& ctx = SslContext::getInstance();
        IpSocket nextClient = listenerSocket.accept();
        SslSession session = ctx.makeSession();
        session.bindTo(nextClient);
        
        if (SignalHandling::cleanupRequested.load()) {
            throw std::runtime_error("SSL Daemon killed by signal.");
        }
    }
}

}
}
