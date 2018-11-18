#ifndef MP3_EVENT_MP3EVENTVISITOR_HPP
#define MP3_EVENT_MP3EVENTVISITOR_HPP

#include "VolumeChangeEvent.hpp"

namespace elevation {

class Mp3EventVisitor {
public:
    virtual void onVolumeChangeEvent(const VolumeChangeEvent& event) = 0;
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENTVISITOR_HPP
