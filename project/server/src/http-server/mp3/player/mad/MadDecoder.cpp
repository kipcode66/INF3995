// TAKEN AND MODIFIED FROM : https://lauri.xn--vsandi-pxa.com/2013/12/implementing-mp3-player.en.html

#include "MadDecoder.hpp"

#include <stdexcept>

namespace elevation {

std::unique_ptr<MadDecoder> MadDecoder::s_instance(nullptr);

void MadDecoder::createInstance(std::vector<uint8_t> buffer, MadAudioFormatter formatter) {
    if (s_instance == nullptr) {
        s_instance = std::unique_ptr<MadDecoder>(new MadDecoder(std::move(buffer), std::move(formatter)));
    }
    else {
        throw std::runtime_error("Cannot create MadDecoder instance : Already exists");
    }
}

MadDecoder& MadDecoder::getInstance() {
    if (s_instance != nullptr) {
        return *s_instance;
    }
    else {
        throw std::runtime_error("Cannot get MadDecoder instance : Not yet created");
    }
}

MadDecoder::MadDecoder(std::vector<uint8_t> buffer, MadAudioFormatter formatter)
    : m_buffer(std::move(buffer))
    , m_formatter(std::move(formatter))
{
    setupLibmad_();
    setupStreamBuffer_();
}

MadDecoder::~MadDecoder()
{
    tearDownLibmad_();
}

void MadDecoder::decodeNextFrame() {
    // Decode frame from the stream
    if (mad_frame_decode(&m_frame, &m_stream) < 0) {
        if (!MAD_RECOVERABLE(m_stream.error) && m_stream.error != MAD_ERROR_BUFLEN) {
            throw std::runtime_error(mad_stream_errorstr(&m_stream));
        }
    }

    // Synthesize PCM data of frame
    mad_synth_frame(&m_synth, &m_frame);
}

void MadDecoder::setupLibmad_() {
    mad_stream_init(&m_stream);
    mad_synth_init (&m_synth );
    mad_frame_init (&m_frame );
}

void MadDecoder::setupStreamBuffer_() {
    mad_stream_buffer(&m_stream, &m_buffer.at(0), m_buffer.size() * sizeof(m_buffer.at(0)));
}

void MadDecoder::tearDownLibmad_() {
    mad_synth_finish (&m_synth );
    mad_frame_finish (&m_frame );
    mad_stream_finish(&m_stream);
}

} // namespace elevation
