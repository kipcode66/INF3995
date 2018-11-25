#ifndef MP3_EVENT_ELEVATIONPLAYERMP3EVENTVISITOR_HPP
#define MP3_EVENT_ELEVATIONPLAYERMP3EVENTVISITOR_HPP

#include <common/mp3/event/AbstractMp3EventVisitor.hpp>
#include <common/mp3/event/VolumeChangeEvent.hpp>
#include <common/mp3/event/MuteEvent.hpp>
#include <common/mp3/event/UnmuteEvent.hpp>
#include <common/mp3/event/VolumeGetRequest.hpp>
#include <common/mp3/communication/Mp3EventSocket.hpp>
#include <common/logger/Logger.hpp>

#include "mp3/player/pulse/PulseVolume.hpp"

namespace elevation {

class ElevationPlayerMp3EventVisitor : public AbstractMp3EventVisitor {
public:
    explicit ElevationPlayerMp3EventVisitor(Logger& logger, std::shared_ptr<Mp3EventSocket> socket);

    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event) override;
    virtual void onMuteEvent        (const MuteEvent&         event) override;
    virtual void onUnmuteEvent      (const UnmuteEvent&       event) override;
    virtual void onVolumeGetRequest (const VolumeGetRequest&  event) override;
    virtual void onVolumeGetResponse(const VolumeGetResponse& event) override;

protected:
    Logger& m_logger;
    std::shared_ptr<Mp3EventSocket> m_socket;
    PulseVolume m_pulseVolume;
};

} // namespace elevation

#endif // !MP3_EVENT_ELEVATIONPLAYERMP3EVENTVISITOR_HPP
