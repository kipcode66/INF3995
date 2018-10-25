// TAKEN AND MODIFIED FROM : https://lauri.xn--vsandi-pxa.com/2013/12/implementing-mp3-player.en.html

#include "MadAudioFormatter.hpp"

#include <stdexcept>

namespace elevation {

MadAudioFormatter::MadAudioFormatter()
    : m_ditheringDistribution(DITHERING_MIN, DITHERING_MAX)
{ }

MadAudioFormatter::~MadAudioFormatter() { }

std::vector<uint8_t> MadAudioFormatter::format(struct mad_pcm* pcm) {
    constexpr std::size_t NUM_BYTES_PER_SAMPLE = 2 * 2;

    std::size_t nsamples = pcm->length;
    mad_fixed_t const* left_ch  = pcm->samples[0];
    mad_fixed_t const* right_ch = pcm->samples[1];

    std::vector<uint8_t> data;
    data.reserve(NUM_BYTES_PER_SAMPLE * nsamples); // Reserve the right size in advance so no
                                                   // copy occurs when we push_back.

    if (pcm->channels == 2) {
        while (nsamples--) {
            mad_fixed_t sample;

            sample = formatSample_(*left_ch++);
            data.push_back((sample >> 0) & 0xff);
            data.push_back((sample >> 8) & 0xff);

            sample = formatSample_(*right_ch++);
            data.push_back((sample >> 0) & 0xff);
            data.push_back((sample >> 8) & 0xff);
        }
    } else {
        throw std::runtime_error("MP3 file does not support stereo");
    }

    return data;
}

mad_fixed_t MadAudioFormatter::formatSample_(mad_fixed_t sample) {
    return dither_(scale_(sample));
}

mad_fixed_t MadAudioFormatter::scale_(mad_fixed_t sample) {
     /* round */
     sample += (1L << (MAD_F_FRACBITS - 16));
     /* clip */
     if (sample >= MAD_F_ONE)
         sample = MAD_F_ONE - 1;
     else if (sample < -MAD_F_ONE)
         sample = -MAD_F_ONE;
     /* quantize */
     return sample >> (MAD_F_FRACBITS + 1 - 16); // NOTE : Right-shifting a negative value is undefined
                                                 // behavior, although it generally performs a sign extension.
                                                 // Oh well.
}

mad_fixed_t MadAudioFormatter::dither_(mad_fixed_t sample) {
    return sample + m_ditheringDistribution(m_rng); // Simple white noise.
}

} // namespace elevation
