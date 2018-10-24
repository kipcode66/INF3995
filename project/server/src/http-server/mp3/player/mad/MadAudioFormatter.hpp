#ifndef MP3_PLAYER_FORMAT_AUDIOFORMATTER_HPP
#define MP3_PLAYER_FORMAT_AUDIOFORMATTER_HPP

#include <inttypes.h>
#include <vector>

#include <mad.h>

namespace elevation {

class MadAudioFormatter {
public:
    MadAudioFormatter();
    ~MadAudioFormatter();

    std::vector<uint8_t> format(struct mad_pcm* pcm);

protected:
    mad_fixed_t scale_(mad_fixed_t sample);
};

} // namespace elevation

#endif // !MP3_PLAYER_FORMAT_AUDIOFORMATTER_HPP
