#include "ElevationPlayerMp3EventVisitor.hpp"

#include <iostream>

namespace elevation {

void ElevationPlayerMp3EventVisitor::onVolumeChangeEvent(const VolumeChangeEvent& event) const {
    std::cout << "Got volume change event to " << (std::size_t)event.getNewVolume() << std::endl;
}

void ElevationPlayerMp3EventVisitor::onMuteEvent(const MuteEvent& event) const {
    std::cout << "Got mute event" << std::endl;
}

void ElevationPlayerMp3EventVisitor::onUnmuteEvent(const UnmuteEvent& event) const {
    std::cout << "Got unmute event" << std::endl;
}

} // namespace elevation
