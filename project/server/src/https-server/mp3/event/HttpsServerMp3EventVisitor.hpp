#ifndef MP3_EVENT_HTTPSSERVERMP3EVENTVISITOR_HPP
#define MP3_EVENT_HTTPSSERVERMP3EVENTVISITOR_HPP

#include <common/mp3/event/AbstractMp3EventVisitor.hpp>
#include <common/mp3/event/VolumeChangeEvent.hpp>
#include <common/mp3/event/MuteEvent.hpp>
#include <common/mp3/event/UnmuteEvent.hpp>
#include <common/mp3/event/VolumeGetRequest.hpp>
#include <common/mp3/event/StopSongEvent.hpp>
#include <common/logger/Logger.hpp>

namespace elevation {

class HttpsServerVolumeGetRequestAdapter;

class HttpsServerMp3EventVisitor : public AbstractMp3EventVisitor {
public:
    explicit HttpsServerMp3EventVisitor(Logger& logger, std::shared_ptr<HttpsServerVolumeGetRequestAdapter> volumeGetRequestAdapter);

    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event) override;
    virtual void onMuteEvent        (const MuteEvent&         event) override;
    virtual void onUnmuteEvent      (const UnmuteEvent&       event) override;
    virtual void onVolumeGetRequest (const VolumeGetRequest&  event) override;
    virtual void onVolumeGetResponse(const VolumeGetResponse& event) override;
    virtual void onStopSongEvent    (const StopSongEvent&     event) override;

protected:
    Logger& m_logger;
    std::shared_ptr<HttpsServerVolumeGetRequestAdapter> m_volumeGetRequestAdapter;
};

} // namespace elevation

#endif // !MP3_EVENT_HTTPSSERVERMP3EVENTVISITOR_HPP
