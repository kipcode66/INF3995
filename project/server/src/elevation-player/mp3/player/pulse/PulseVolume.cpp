#include "PulseVolume.hpp"

namespace elevation {

PulseVolume::PulseVolume(::pa_context* context)
    : m_context(context)
{ }

volumePercent_t PulseVolume::getVolume() const {
    return 255;
}

void PulseVolume::setVolume(volumePercent_t newVolume) {

}

void PulseVolume::mute() {

}

void PulseVolume::unmute() {

}

} // namespace elevation
