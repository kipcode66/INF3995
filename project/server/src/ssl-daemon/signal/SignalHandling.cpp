#include "SignalHandling.hpp"

#include <signal.h>
#include <unistd.h>

namespace elevation {
namespace daemon {

std::atomic<bool> SignalHandling::cleanupRequested(false);

void SignalHandling::installSignalHandlers() {
    installSignalHandlerFor_(SIGINT);
    installSignalHandlerFor_(SIGTERM);
}

void SignalHandling::installSignalHandlerFor_(int signal) {
    // From https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html
    struct sigaction newAction, oldAction;
    newAction.sa_handler = &SignalHandling::signalHandler_;
    ::sigaction(signal, &newAction, &oldAction);
}

void SignalHandling::signalHandler_(int signal) {
    cleanupRequested.store(true);

    // Don't reinstall the signal handler : if the code doesn't check the flag
    // for a while, we want to be able to terminate it by sending the same signal.
}

} // namespace daemon
} // namespace elevation

