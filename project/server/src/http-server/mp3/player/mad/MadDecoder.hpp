#ifndef MP3_PLAYER_MAD_MADDECODER_HPP
#define MP3_PLAYER_MAD_MADDECODER_HPP

#include <vector>
#include <inttypes.h>
#include <memory>

#include <mad.h>

namespace elevation {

class MadDecoder {
public:
    static void createInstance(std::vector<uint8_t> buffer);
    static MadDecoder& getInstance();

public:
    virtual ~MadDecoder();

    void decodeNextFrame();

private:
    explicit MadDecoder(std::vector<uint8_t> buffer);

protected:
    void setupLibmad_();
    void setupStreamBuffer_();
    void tearDownLibmad_();

protected:
    std::vector<uint8_t> m_buffer;
    struct ::mad_stream m_stream;
    struct ::mad_frame m_frame;
    struct ::mad_synth m_synth;

protected:
    static std::unique_ptr<MadDecoder> s_instance;
};

} // namespace elevation

#endif // !MP3_PLAYER_MAD_MADDECODER_HPP
