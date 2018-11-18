#ifndef MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP
#define MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP

#include <common/mp3/event/Mp3EventVisitor.hpp>

namespace elevation {

class ElevationPlayerMp3EventVisitor : public Mp3EventVisitor {
public:
    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event);
};

} // namespace elevation

#endif // !MP3_EVENT_ELEVATION_PLAYERMP3EVENTVISITOR_HPP
