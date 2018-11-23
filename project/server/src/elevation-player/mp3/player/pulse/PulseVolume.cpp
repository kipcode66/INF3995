// Partly taken from https://www.freedesktop.org/wiki/Software/PulseAudio/Documentation/Developer/Clients/Samples/AsyncDeviceList

#include "PulseVolume.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <cmath>


#include <chrono>
#include <thread>

namespace elevation {

double PulseVolume::toLogScale_(double linearFactor) {
    static const double A =
        F0
        *
        (
            1.0
            -
            (
                (1.0 - F0)
                /
                (
                    F0
                    *
                    (std::pow(10.0, 1.0/K) - 1.0)
                )
            )
        );

    static const double B =
        (std::pow(10.0, 1.0/K) - 1.0)
        /
        (1.0 - F0);

    if (linearFactor < 0) {
        throw std::logic_error(std::string{"Cannot convert "} + std::to_string(linearFactor) + " to log scale : Factor is negative");
    }
    if (linearFactor < F0) {
        linearFactor = F0;
    }
    return K * std::log10((linearFactor - A) * B);
}
}

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
    volumePercent_t volume;

    pa_sink_info_cb_t callback = [](pa_context *c, const pa_sink_info *i, int eol, void* data) {
        if (eol <= 0) {
            volumePercent_t* volume = static_cast<volumePercent_t*>(data);
            const pa_cvolume* sinkVolumes = &i->volume;
            double sinkVolume = pa_sw_volume_to_linear(pa_cvolume_avg(sinkVolumes));
            *volume = static_cast<volumePercent_t>(toLogScale_(sinkVolume) * 100.0);
        }
    };
    ::pa_operation* sinkLinkRequestOperation = ::pa_context_get_sink_info_list(m_context, callback, (void*)&volume);
    while (::pa_operation_get_state(sinkLinkRequestOperation) != PA_OPERATION_DONE) {
        ::pa_mainloop_iterate(m_mainloop, 1, NULL);
    }
    ::pa_operation_unref(sinkLinkRequestOperation);
    return volume;
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
    pa_context_connect(m_context, NULL, PA_CONTEXT_NOFLAGS, NULL);

    pa_context_notify_cb_t stateChangeCallback_ =
        [](::pa_context *c, void *userdata) {
            ::pa_context_state_t state;
            int* pa_ready = static_cast<int*>(userdata);

            state = ::pa_context_get_state(c);
            switch  (state) {
                case PA_CONTEXT_UNCONNECTED:
                case PA_CONTEXT_CONNECTING:
                case PA_CONTEXT_AUTHORIZING:
                case PA_CONTEXT_SETTING_NAME:
                default:
                        break;
                case PA_CONTEXT_FAILED:
                case PA_CONTEXT_TERMINATED:
                        *pa_ready = 2;
                        break;
                case PA_CONTEXT_READY:
                        *pa_ready = 1;
                        break;
            }
        };

    int pa_ready = 0;
    ::pa_context_set_state_callback(m_context, stateChangeCallback_, &pa_ready);
    while (pa_ready == 0) {
        ::pa_mainloop_iterate(m_mainloop, 1, NULL);
    }
}

} // namespace elevation
