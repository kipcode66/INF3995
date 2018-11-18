#ifndef MP3_EVENT_ABSTRACTMP3EVENTVISITOR_HPP
#define MP3_EVENT_ABSTRACTMP3EVENTVISITOR_HPP

namespace elevation {

class VolumeChangeEvent; // Forward-declaration to break circular dependencies.

class AbstractMp3EventVisitor {
public:
    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event) const = 0;
};

} // namespace elevation

#endif // !MP3_EVENT_ABSTRACTMP3EVENTVISITOR_HPP
