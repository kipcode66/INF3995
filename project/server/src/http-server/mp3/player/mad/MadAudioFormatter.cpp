// TAKEN AND MODIFIED FROM : https://lauri.xn--vsandi-pxa.com/2013/12/implementing-mp3-player.en.html

#include "MadAudioFormatter.hpp"

#include <stdexcept>

namespace elevation {

MadAudioFormatter::MadAudioFormatter() { }

MadAudioFormatter::~MadAudioFormatter() { }

std::vector<uint8_t> MadAudioFormatter::format(struct mad_pcm* pcm) {
    constexpr std::size_t MAX_NUM_SAMPLES = 1152;
    constexpr std::size_t NUM_BYTES_PER_SAMPLE = 2 * 2;
    constexpr std::size_t BUFFER_SIZE = MAX_NUM_SAMPLES * NUM_BYTES_PER_SAMPLE;

    std::size_t nsamples = pcm->length;
    mad_fixed_t const* left_ch  = pcm->samples[0];
    mad_fixed_t const* right_ch = pcm->samples[1];

    std::vector<uint8_t> data;
    data.resize(NUM_BYTES_PER_SAMPLE * nsamples);

    if (pcm->channels == 2) {
        while (nsamples--) {
            mad_fixed_t sample;

            sample = scale_(*left_ch++);
            data[(pcm->length - nsamples) * 4 + 0] = ((sample >> 0) & 0xff);
            data[(pcm->length - nsamples) * 4 + 1] = ((sample >> 8) & 0xff);

            sample = scale_(*right_ch++);
            data[(pcm->length - nsamples) * 4 + 2] = ((sample >> 0) & 0xff);
            data[(pcm->length - nsamples) * 4 + 3] = ((sample >> 8) & 0xff);
        }
    } else {
        throw std::runtime_error("Libmad does not support stereo");
    }
    
    return data;
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

} // namespace elevation
