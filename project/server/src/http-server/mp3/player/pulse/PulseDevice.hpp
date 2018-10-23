#ifndef MP3_PLAYER_PULSE_PULSEDEVICE_HPP
#define MP3_PLAYER_PULSE_PULSEDEVICE_HPP

#include <pulse/simple.h>
#include <pulse/error.h>

namespace elevation {

class PulseDevice {
public:
    static constexpr pa_sample_spec PULSE_SPECIFICATION = { .format = PA_SAMPLE_S16LE, .rate = 44100, .channels = 2 };

public:
    ~PulseDevice();

    PulseDevice(const PulseDevice&) = delete;
    PulseDevice(PulseDevice&& that);

    PulseDevice& operator=(const PulseDevice&) = delete;
    PulseDevice& operator=(PulseDevice&& that);

private:
    PulseDevice();

protected:
    void cleanup_();

protected:
    pa_simple* m_device;
};

} // namespace elevation

#endif // !MP3_PLAYER_PULSE_PULSEDEVICE_HPP
