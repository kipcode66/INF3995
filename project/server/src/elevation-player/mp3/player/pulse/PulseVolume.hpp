#ifndef MP3_PLAYER_PULSE_PULSEVOLUME_HPP
#define MP3_PLAYER_PULSE_PULSEVOLUME_HPP

#include <memory>

#include <pulse/pulseaudio.h>

#include <common/mp3/definitions/Volume.hpp>

namespace elevation {

class PulseDevice; // Forward-declaration to brea circular dependencies

class PulseVolume {
protected:
    static double getMinVolumeLogScale_() { return toLogScale_(0); }
    static double getMaxVolumeLogScale_() { return toLogScale_(1); }
    static double toLogScale_(double linearFactor);

public:
    explicit PulseVolume();
    PulseVolume(const PulseVolume&) = delete;
    PulseVolume(PulseVolume&&) = delete;
    virtual ~PulseVolume();

    PulseVolume& operator=(const PulseVolume&) = delete;
    PulseVolume& operator=(PulseVolume&&) = delete;

    volumePercent_t getVolume() const;
    void setVolume(volumePercent_t newVolume);
    void mute();
    void unmute();

protected:
    void initializeContext_();

protected:
    ::pa_context* m_context;
    ::pa_mainloop* m_mainloop;
};

} // namespace elevation

#endif // !MP3_PLAYER_PULSE_PULSEVOLUME_HPP
