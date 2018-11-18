#include "ElevationPlayerMp3EventVisitor.hpp"

#include <iostream>

namespace elevation {

void ElevationPlayerMp3EventVisitor::onVolumeChangeEvent(const VolumeChangeEvent& event) {
    std::cout << "Got volume change event to " << event.getNewVolume() << std::endl;
}

} // namespace elevation
