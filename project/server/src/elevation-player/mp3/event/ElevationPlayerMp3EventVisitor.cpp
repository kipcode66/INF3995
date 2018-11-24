#include "ElevationPlayerMp3EventVisitor.hpp"

#include <iostream>

#include <common/mp3/event/exception/UnexpectedEventException.hpp>

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
    // TODO Do something in reaction
    m_logger.log("Got volume get request event");
}

void ElevationPlayerMp3EventVisitor::onVolumeGetResponse(const VolumeGetResponse& event) {
    m_logger.err("Got unexpected volume get response event");
    throw UnexpectedEventException("VolumeGetResponse");
}

} // namespace elevation
