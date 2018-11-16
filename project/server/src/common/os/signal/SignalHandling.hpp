#ifndef SIGNAL_SIGNALHANDLING_HPP
#define SIGNAL_SIGNALHANDLING_HPP

#include <atomic>

namespace elevation {

class SignalHandling {
public:
    static void installSignalHandlers();
    static bool isCleanupRequested();

private:
    static void installSignalHandlerFor_(int signal);

private:
    SignalHandling();
    ~SignalHandling();
};

} // namespace elevation

#endif // !SIGNAL_SIGNALHANDLING_HPP
