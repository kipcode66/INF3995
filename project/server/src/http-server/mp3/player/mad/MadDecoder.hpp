#ifndef MP3_PLAYER_MAD_MADDECODER_HPP
#define MP3_PLAYER_MAD_MADDECODER_HPP

#include <vector>
#include <inttypes.h>
#include <memory>

#include <mad.h>

#include "MadAudioFormatter.hpp"
#include "os/SharedFileMemory.hpp"

namespace elevation {

class MadDecoder {
public:
    explicit MadDecoder(SharedFileMemory fileMemory, MadAudioFormatter formatter);
    MadDecoder(const MadDecoder&) = delete;
    MadDecoder(MadDecoder&& that);

    virtual ~MadDecoder();

    MadDecoder& operator=(const MadDecoder&) = delete;
    MadDecoder& operator=(MadDecoder&& that);

    std::vector<uint8_t> decodeNextFrame();
    bool isDone() const { return m_isDone; }

protected:
    void setupLibmad_();
    void setupStreamBuffer_();
    void tearDownLibmad_();

protected:
    struct ::mad_stream m_stream;
    struct ::mad_frame m_frame;
    struct ::mad_synth m_synth;

    SharedFileMemory m_fileMemory;
    MadAudioFormatter m_formatter;

    bool m_isDone;
};

} // namespace elevation

#endif // !MP3_PLAYER_MAD_MADDECODER_HPP
