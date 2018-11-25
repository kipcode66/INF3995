#include "ElevationPlayerMp3EventVisitor.hpp"

#include <iostream>

namespace elevation {

void ElevationPlayerMp3EventVisitor::onVolumeChangeEvent(const VolumeChangeEvent& event) const {
    std::cout << "Got volume change event to " << (std::size_t)event.getNewVolume() << std::endl;
}

} // namespace elevation
