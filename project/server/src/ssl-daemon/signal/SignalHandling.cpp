#include "SignalHandling.hpp"

namespace elevation {
namespace daemon {

std::atomic<bool> SignalHandling::cleanupRequested(false);

} // namespace daemon
} // namespace elevation

