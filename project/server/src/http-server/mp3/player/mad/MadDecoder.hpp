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
    static void createInstance(SharedFileMemory fileMemory, MadAudioFormatter formatter);
    static MadDecoder& getInstance();

public:
    virtual ~MadDecoder();

    std::vector<uint8_t> decodeNextFrame();

private:
    explicit MadDecoder(SharedFileMemory fileMemory, MadAudioFormatter formatter);

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

protected:
    static std::unique_ptr<MadDecoder> s_instance;
};

} // namespace elevation

#endif // !MP3_PLAYER_MAD_MADDECODER_HPP
