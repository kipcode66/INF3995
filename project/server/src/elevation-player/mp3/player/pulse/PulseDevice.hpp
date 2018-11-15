#ifndef MP3_PLAYER_PULSE_PULSEDEVICE_HPP
#define MP3_PLAYER_PULSE_PULSEDEVICE_HPP

#include <vector>

#include <pulse/simple.h>
#include <pulse/error.h>

namespace elevation {

class PulseDevice {
public:
    static const pa_sample_spec PULSE_SPECIFICATION;

public:
    PulseDevice();
    PulseDevice(const PulseDevice&) = delete;
    PulseDevice(PulseDevice&& that);

    ~PulseDevice();

    PulseDevice& operator=(const PulseDevice&) = delete;
    PulseDevice& operator=(PulseDevice&& that);

    void play(const std::vector<uint8_t>& data);

protected:
    void cleanup_();

protected:
    pa_simple* m_device;
};

} // namespace elevation

#endif // !MP3_PLAYER_PULSE_PULSEDEVICE_HPP
