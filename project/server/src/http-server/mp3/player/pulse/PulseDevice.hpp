#ifndef MP3_PLAYER_PULSE_PULSEDEVICE_HPP
#define MP3_PLAYER_PULSE_PULSEDEVICE_HPP

#include <pulse/simple.h>
#include <pulse/error.h>

namespace elevation {

class PulseController; // Because of forward dependencies.

class PulseDevice {
    friend class PulseController;

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

#include "PulseController.hpp" // Could not include above because of forward dependencies.

#endif // !MP3_PLAYER_PULSE_PULSEDEVICE_HPP
