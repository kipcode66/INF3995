#ifndef MP3_PLAYER_FORMAT_AUDIOFORMATTER_HPP
#define MP3_PLAYER_FORMAT_AUDIOFORMATTER_HPP

#include <inttypes.h>
#include <vector>
#include <random>

#include <mad.h>

namespace elevation {

class MadAudioFormatter {
public:
    static constexpr mad_fixed_t DITHERING_MAX = (1 << 16) / 500;
    static constexpr mad_fixed_t DITHERING_MIN = -DITHERING_MAX;

public:
    MadAudioFormatter();
    ~MadAudioFormatter();

    std::vector<uint8_t> format(struct mad_pcm* pcm);

protected:
    mad_fixed_t formatSample_(mad_fixed_t sample);
    mad_fixed_t scale_(mad_fixed_t sample);
    mad_fixed_t dither_(mad_fixed_t sample);

protected:
    std::default_random_engine m_rng;
    std::uniform_int_distribution<mad_fixed_t> m_ditheringDistribution;
};

} // namespace elevation

#endif // !MP3_PLAYER_FORMAT_AUDIOFORMATTER_HPP
