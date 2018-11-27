#include "ElevationPlayerMp3EventVisitor.hpp"

#include <iostream>

#include <common/mp3/event/VolumeGetResponse.hpp>
#include <common/mp3/event/exception/UnexpectedEventException.hpp>
namespace elevation {

ElevationPlayerMp3EventVisitor::ElevationPlayerMp3EventVisitor(Logger& logger, std::shared_ptr<Mp3EventSocket> socket, std::shared_ptr<Mp3AutoPlayer> autoPlayer)
    : m_logger(logger)
    , m_socket(socket)
    , m_autoPlayer(autoPlayer)
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
    VolumeData_t volumeData = {
        .volume = m_pulseVolume.getVolume(),
        .isMuted = m_pulseVolume.isMuted()
    };
    VolumeGetResponse response{volumeData};
    m_socket->writeEvent(response);
}

void ElevationPlayerMp3EventVisitor::onVolumeGetResponse(const VolumeGetResponse& event) {
    m_logger.err("Got unexpected volume get response event");
    throw UnexpectedEventException("VolumeGetResponse");
}

void ElevationPlayerMp3EventVisitor::onStopSongEvent(const StopSongEvent& event) {
    m_autoPlayer->stopSong();
    m_logger.log("Got song stop event");
}

} // namespace elevation
