#ifndef MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP
#define MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP

#include <common/mp3/event/AbstractMp3EventVisitor.hpp>
#include <common/mp3/event/VolumeChangeEvent.hpp>

namespace elevation {

class ElevationPlayerMp3EventVisitor : public AbstractMp3EventVisitor {
public:
    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event) const override;
    virtual void onMuteEvent(const MuteEvent& event) const override;
    virtual void onUnmuteEvent(const UnmuteEvent& event) const override;
};

} // namespace elevation

#endif // !MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP
