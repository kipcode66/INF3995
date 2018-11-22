#ifndef MP3_EVENT_ABSTRACTMP3EVENTVISITOR_HPP
#define MP3_EVENT_ABSTRACTMP3EVENTVISITOR_HPP

namespace elevation {

// Forward-declarations to break circular dependencies.
class VolumeChangeEvent;
class MuteEvent;

class AbstractMp3EventVisitor {
public:
    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event) const = 0;
    virtual void onMuteEvent(const MuteEvent& event) const = 0;
};

} // namespace elevation

#endif // !MP3_EVENT_ABSTRACTMP3EVENTVISITOR_HPP
