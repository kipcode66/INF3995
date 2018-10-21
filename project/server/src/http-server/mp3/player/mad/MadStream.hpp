#ifndef MP3_PLAYER_MAD_MADSTREAM_HPP
#define MP3_PLAYER_MAD_MADSTREAM_HPP

#include <vector>
#include <inttypes.h>
#include <memory>

#include <mad.h>

namespace elevation {

class MadStream {
public:
    static void createInstance(std::vector<uint8_t> buffer);
    static MadStream& getInstance();

public:
    virtual ~MadStream();

private:
    explicit MadStream(std::vector<uint8_t> buffer);

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
    static std::unique_ptr<MadStream> s_instance;
};

}

#endif // !MP3_PLAYER_MAD_MADSTREAM_HPP
