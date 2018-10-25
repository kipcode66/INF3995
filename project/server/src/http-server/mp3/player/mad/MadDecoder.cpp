// TAKEN AND MODIFIED FROM : https://lauri.xn--vsandi-pxa.com/2013/12/implementing-mp3-player.en.html

#include "MadDecoder.hpp"

#include <stdexcept>

namespace elevation {

MadDecoder::MadDecoder(SharedFileMemory fileMemory, MadAudioFormatter formatter)
    : m_fileMemory(std::move(fileMemory))
    , m_formatter(std::move(formatter))
    , m_isDone(false)
{
    setupLibmad_();
    setupStreamBuffer_();
}

MadDecoder::MadDecoder(MadDecoder&& that) 
    : m_fileMemory(std::move(that.m_fileMemory))
    , m_formatter(std::move(that.m_formatter))
    , m_isDone(that.m_isDone)
{
    this->m_stream = std::move(that.m_stream);
    this->m_frame  = std::move(that.m_frame );
    this->m_synth  = std::move(that.m_synth );
}

MadDecoder::~MadDecoder()
{
    tearDownLibmad_();
}

MadDecoder& MadDecoder::operator=(MadDecoder&& that) {
    tearDownLibmad_();
    this->m_fileMemory = std::move(that.m_fileMemory);
    this->m_formatter  = std::move(that.m_formatter );
    this->m_stream = std::move(that.m_stream);
    this->m_frame  = std::move(that.m_frame );
    this->m_synth  = std::move(that.m_synth );
    this->m_isDone = that.m_isDone;
    that .m_isDone = true;
    return *this;
}

std::vector<uint8_t> MadDecoder::decodeNextFrame() {
    // Decode frame from the stream
    int error = ::mad_frame_decode(&m_frame, &m_stream);
    if (error != 0) {
        if (!MAD_RECOVERABLE(m_stream.error)) {
            if (m_stream.error == MAD_ERROR_BUFLEN) {
                m_isDone = true;
            }
            else {
                throw std::runtime_error(mad_stream_errorstr(&m_stream));
            }
        }
    }
    else {
        // Synthesize PCM data of frame
        mad_synth_frame(&m_synth, &m_frame);
        return m_formatter.format(&m_synth.pcm);
    }
    return std::vector<uint8_t>();
}

void MadDecoder::setupLibmad_() {
    ::mad_stream_init(&m_stream);
    ::mad_synth_init (&m_synth );
    ::mad_frame_init (&m_frame );
}

void MadDecoder::setupStreamBuffer_() {
    ::mad_stream_buffer(&m_stream, (uint8_t*)m_fileMemory, m_fileMemory.size() * sizeof(*(uint8_t*)m_fileMemory));
}

void MadDecoder::tearDownLibmad_() {
      mad_synth_finish (&m_synth );
    ::mad_frame_finish (&m_frame );
    ::mad_stream_finish(&m_stream);
}

} // namespace elevation
