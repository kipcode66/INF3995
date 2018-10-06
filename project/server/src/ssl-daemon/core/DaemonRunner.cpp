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
    SslContext::createInstance(m_config.getListenPort());
    SslContext& ctx = SslContext::getInstance();

    while (true) {
        SslSession session = ctx.acceptSession();
        std::cout << "Accepted HTTPS connection." << std::endl;
        
        if (SignalHandling::cleanupRequested.load()) {
            throw std::runtime_error("SSL Daemon killed by signal.");
        }
    }
}

}
}
