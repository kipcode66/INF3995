#ifndef MP3_PLAYER_PULSE_PULSEVOLUME_HPP
#define MP3_PLAYER_PULSE_PULSEVOLUME_HPP

#include <memory>

#include <pulse/context.h>

#include <common/mp3/definitions/Volume.hpp>

namespace elevation {

class PulseDevice; // Forward-declaration to brea circular dependencies

class PulseVolume {
public:
    explicit PulseVolume(::pa_context* context);
    virtual ~PulseVolume() = default;

    volumePercent_t getVolume() const;
    void setVolume(volumePercent_t newVolume);
    void mute();
    void unmute();

protected:
    ::pa_context* m_context;
};

} // namespace elevation

#endif // !MP3_PLAYER_PULSE_PULSEVOLUME_HPP
