// TAKEN AND MODIFIED FROM : https://lauri.xn--vsandi-pxa.com/2013/12/implementing-mp3-player.en.html

#include "PulseDevice.hpp"

#include <stdexcept>
#include <utility>

namespace elevation {

const pa_sample_spec PulseDevice::PULSE_SPECIFICATION = { .format = PA_SAMPLE_S16LE, .rate = 44100, .channels = 2 };

PulseDevice::PulseDevice()
    : m_device(nullptr)
    , m_pulseVolume()
{
    int error;
    pa_simple* device = pa_simple_new(NULL, "Elevation player", PA_STREAM_PLAYBACK, NULL, "Elevation stream", &PULSE_SPECIFICATION, NULL, NULL, &error);
    if (device == NULL) {
        throw std::runtime_error(::pa_strerror(error));
    }
    m_device = device;
}

PulseDevice::PulseDevice(PulseDevice&& that)
    : m_device(nullptr)
    , m_pulseVolume()
{
    operator=(std::move(that));
}

PulseDevice::~PulseDevice()
{
    cleanup_();
}

PulseDevice& PulseDevice::operator=(PulseDevice&& that) {
    cleanup_();
    this->m_device = that.m_device;
    that .m_device = nullptr;
    return *this;
}

void PulseDevice::play(const std::vector<uint8_t>& data) {
    int error;
    if (data.size() > 0) {
        if (::pa_simple_write(m_device, &data.at(0), data.size() * sizeof(data.at(0)), &error) < 0) {
            throw std::runtime_error(::pa_strerror(error));
        }
    }
}

void PulseDevice::cleanup_() {
    if (m_device != nullptr) {
        ::pa_simple_free(m_device);
    }
}

} // namespace elevation
