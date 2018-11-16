#include "SignalHandling.hpp"

#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <stdexcept>

static std::atomic<struct sigaction> oldSigaction;

extern "C" void signalHandler_(int signal) {
    elevation::SignalHandling::s_cleanupRequested.store(true);

    static char message[] = "Caught signal.\nIf the SSL Daemon does not stop immediately, "
                            "it will on the next HTTPS connection, or if sending the same "
                            "signal another time.\n";
    ::write(STDERR_FILENO, message, sizeof(message));

    struct sigaction newSigaction = oldSigaction.load();
    ::sigaction(signal, &newSigaction, NULL); // Install old sigaction instead
}

namespace elevation {

std::atomic<bool> SignalHandling::s_cleanupRequested(false);

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

} // namespace elevation

