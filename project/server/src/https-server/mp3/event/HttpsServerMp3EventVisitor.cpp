#include "HttpsServerMp3EventVisitor.hpp"

#include <iostream>

#include <common/mp3/event/VolumeGetResponse.hpp>
#include <common/mp3/event/exception/UnexpectedEventException.hpp>

#include "HttpsServerVolumeGetRequestAdapter.hpp"

namespace elevation {

HttpsServerMp3EventVisitor::HttpsServerMp3EventVisitor(Logger& logger, std::shared_ptr<HttpsServerVolumeGetRequestAdapter> volumeGetRequestAdapter)
    : m_logger(logger)
    , m_volumeGetRequestAdapter(volumeGetRequestAdapter)
{ }

void HttpsServerMp3EventVisitor::onVolumeChangeEvent(const VolumeChangeEvent& event) {
    m_logger.err("Got unexpected volume change event");
    throw UnexpectedEventException("VolumeChangeEvent");
}

void HttpsServerMp3EventVisitor::onMuteEvent(const MuteEvent& event) {
    m_logger.err("Got unexpected mute event");
    throw UnexpectedEventException("MuteEvent");
}

void HttpsServerMp3EventVisitor::onUnmuteEvent(const UnmuteEvent& event) {
    m_logger.err("Got unexpected unmute event");
    throw UnexpectedEventException("UnmuteEvent");
}

void HttpsServerMp3EventVisitor::onVolumeGetRequest(const VolumeGetRequest& event) {
    m_logger.err("Got unexpected volume get request event");
    throw UnexpectedEventException("VolumeGetRequest");
}

void HttpsServerMp3EventVisitor::onVolumeGetResponse(const VolumeGetResponse& event) {
    m_volumeGetRequestAdapter->setVolumeData_(event.getVolumeData());
    m_logger.log("Got volume get response");
}

} // namespace elevation
