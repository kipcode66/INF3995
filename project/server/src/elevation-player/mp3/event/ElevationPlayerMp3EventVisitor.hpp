#ifndef MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP
#define MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP

#include <common/mp3/event/AbstractMp3EventVisitor.hpp>
#include <common/mp3/event/VolumeChangeEvent.hpp>
#include <common/logger/Logger.hpp>

#include "mp3/player/pulse/PulseVolume.hpp"

namespace elevation {

class ElevationPlayerMp3EventVisitor : public AbstractMp3EventVisitor {
public:
    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event) override;
    virtual void onMuteEvent(const MuteEvent& event) override;
    virtual void onUnmuteEvent(const UnmuteEvent& event) override;
};

} // namespace elevation

#endif // !MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP
