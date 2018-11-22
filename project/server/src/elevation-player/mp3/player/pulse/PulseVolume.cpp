#include "PulseVolume.hpp"

namespace elevation {

PulseVolume::PulseVolume()
{
    initializeContext_();
}

PulseVolume::~PulseVolume() {
    if(m_context != nullptr) {
        pa_context_unref(m_context);
    }
    if (m_mainloop != nullptr) {
        pa_mainloop_free(m_mainloop);
    }
}

volumePercent_t PulseVolume::getVolume() const {
    return 255;
}

void PulseVolume::setVolume(volumePercent_t newVolume) {

}

void PulseVolume::mute() {

}

void PulseVolume::unmute() {

}

void PulseVolume::initializeContext_() {
    m_mainloop = ::pa_mainloop_new();
    m_context = ::pa_context_new(::pa_mainloop_get_api(m_mainloop), nullptr);
}

} // namespace elevation
