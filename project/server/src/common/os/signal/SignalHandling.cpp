#include "SignalHandling.hpp"

#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <stdexcept>

static std::atomic<struct sigaction> oldSigaction;
static std::atomic<bool> cleanupRequested{false}; ///< @brief Whether a signal such as SIGINT requested a cleanup.

extern "C" void signalHandler_(int signal) {
    cleanupRequested.store(true);

    static char message[] = "Caught signal.\nIf the SSL Daemon does not stop immediately, "
                            "it will on the next HTTPS connection, or if sending the same "
                            "signal another time.\n";
    ::write(STDERR_FILENO, message, sizeof(message));

    struct sigaction newSigaction = oldSigaction.load();
    ::sigaction(signal, &newSigaction, NULL); // Install old sigaction instead
}

namespace elevation {

void SignalHandling::installSignalHandlers() {
    installSignalHandlerFor_(SIGINT);
    installSignalHandlerFor_(SIGTERM);
}

bool SignalHandling::isCleanupRequested() {
    return cleanupRequested.load();
}

void SignalHandling::installSignalHandlerFor_(int signal) {
    // From https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html
    struct sigaction newAction, oldAction;
    newAction.sa_handler = &signalHandler_;
    ::sigaction(signal, NULL, &oldAction); // Get sigaction that will be replaced
    ::oldSigaction.store(oldAction);
    ::sigaction(signal, &newAction, NULL); // Set new sigaction
}

} // namespace elevation

