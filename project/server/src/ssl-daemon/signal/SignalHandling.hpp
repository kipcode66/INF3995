#ifndef SIGNAL_SIGNAL_HANDLING_HPP
#define SIGNAL_SIGNAL_HANDLING_HPP

#include <atomic>

namespace elevation {
namespace daemon {

class SignalHandling {
public:
    static std::atomic<bool> cleanupRequested; ///< Whether A signal such as SIGINT requested a cleanup.
    static void installSignalHandlers();

private:
    static void installSignalHandlerFor_(int signal);
    static void signalHandler_(int signal);

private:
    SignalHandling();
    ~SignalHandling();
};

} // namespace daemon
} // namespace elevation

#endif // !SIGNAL_SIGNAL_HANDLING_HPP
