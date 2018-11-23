// Partly taken from https://www.freedesktop.org/wiki/Software/PulseAudio/Documentation/Developer/Clients/Samples/AsyncDeviceList

#include "PulseVolume.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <cmath>

#include "PulseOperation.hpp"

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

double PulseVolume::fromLogScale_(double logFactor) {
    static const double A =
        (1 - F0)
        /
        (std::pow(10.0, 1.0/K) - 1.0);
    static const double B =
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

    if (logFactor < 0) {
        throw std::logic_error(std::string{"Cannot convert "} + std::to_string(logFactor) + " to linear scale : Factor is negative");
    }

    return A * std::pow(10.0, logFactor / K) + B;
}

PulseVolume::PulseVolume()
{
    initializeContext_();
    initializeSinkData_();
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
    volumePercent_t volume = std::numeric_limits<volumePercent_t>::max();

    pa_sink_info_cb_t callback = [](pa_context *c, const pa_sink_info *i, int eol, void* data) {
        volumePercent_t* volume = static_cast<volumePercent_t*>(data);
        if (eol <= 0) {
            const pa_cvolume* sinkVolumes = &i->volume;
            double sinkVolume = pa_sw_volume_to_linear(pa_cvolume_avg(sinkVolumes));
            *volume = static_cast<volumePercent_t>(toLogScale_(sinkVolume) * 100.0);
        }
    };
    PulseOperation op{::pa_context_get_sink_info_by_index(m_context, m_sinkIndex, callback, (void*)&volume), m_mainloop};
    op.waitUntilCompleteOrFailed();
    return volume;
}

void PulseVolume::setVolume(volumePercent_t newVolume) {
    double sinkVolume = fromLogScale_(static_cast<double>(newVolume) / 100.0);
    ::pa_volume_t pulseInternalVolume = ::pa_sw_volume_from_linear(sinkVolume);
    ::pa_cvolume volume;
    ::pa_cvolume_init(&volume);
    ::pa_cvolume_set(&volume, m_numSinkChannels, pulseInternalVolume);
    ::pa_context_success_cb_t callback = [](pa_context *c, int success, void* data) { };
    PulseOperation op{::pa_context_set_sink_volume_by_index(m_context, m_sinkIndex, &volume, callback, &newVolume), m_mainloop};
    op.waitUntilCompleteOrFailed();
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

void PulseVolume::initializeSinkData_() {
    struct Data_t {
        bool alreadyDone;
        uint32_t sinkIndex;
        uint8_t numSinkChannels;
    };

    Data_t data = {.alreadyDone = false, .sinkIndex = std::numeric_limits<uint32_t>::max()};

    pa_sink_info_cb_t callback = [](pa_context *c, const pa_sink_info *i, int eol, void* dataVoid) {
        Data_t* data = static_cast<Data_t*>(dataVoid);
        if (!data->alreadyDone && eol <= 0) {
            data->sinkIndex = i->index;
            data->numSinkChannels = i->sample_spec.channels;
            data->alreadyDone = true;
        }
    };
    PulseOperation op{::pa_context_get_sink_info_list(m_context, callback, (void*)&data), m_mainloop};
    op.waitUntilCompleteOrFailed();
    m_sinkIndex = data.sinkIndex;
    m_numSinkChannels = data.numSinkChannels;
}

} // namespace elevation
