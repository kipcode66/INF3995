#include "SignalHandling.hpp"

#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <stdexcept>
#include <pthread.h>

static std::atomic<struct sigaction> oldSigaction;

extern "C" void signalHandler_(int signal) {
    elevation::daemon::SignalHandling::cleanupRequested.store(true);

    ::exit(0);

    struct sigaction newSigaction = oldSigaction.load();
    ::sigaction(signal, &newSigaction, NULL); // Install old sigaction instead, in case pthread_exit
                                              // doesn't work because we're stuck on a noncancelling syscall.
}

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
    newAction.sa_handler = &signalHandler_;
    ::sigaction(signal, NULL, &oldAction); // Get sigaction that will be replaced
    ::oldSigaction.store(oldAction);
    ::sigaction(signal, &newAction, NULL); // Set new sigaction
}

} // namespace daemon
} // namespace elevation

