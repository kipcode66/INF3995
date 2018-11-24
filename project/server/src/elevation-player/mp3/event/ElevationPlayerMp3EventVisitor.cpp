#include "ElevationPlayerMp3EventVisitor.hpp"

#include <iostream>

namespace elevation {

ElevationPlayerMp3EventVisitor::ElevationPlayerMp3EventVisitor(Logger& logger)
    : m_logger(logger)
{ }

void ElevationPlayerMp3EventVisitor::onVolumeChangeEvent(const VolumeChangeEvent& event) {
    m_pulseVolume.setVolume(event.getNewVolume());
    std::ostringstream messageStream;
    messageStream << "Got volume change event to " << (std::size_t)event.getNewVolume();
    m_logger.log(messageStream.str());
}

void ElevationPlayerMp3EventVisitor::onMuteEvent(const MuteEvent& event) {
    m_pulseVolume.mute();
    m_logger.log("Got mute event");
}

void ElevationPlayerMp3EventVisitor::onUnmuteEvent(const UnmuteEvent& event) {
    m_pulseVolume.unmute();
    m_logger.log("Got unmute event");
}

void ElevationPlayerMp3EventVisitor::onVolumeGetRequest(const VolumeGetRequest& event) {
    m_logger.log("Got volume get request event");
}

} // namespace elevation
