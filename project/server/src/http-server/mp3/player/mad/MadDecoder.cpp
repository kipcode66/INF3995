#include "MadDecoder.hpp"

#include <stdexcept>

namespace elevation {

std::unique_ptr<MadDecoder> MadDecoder::s_instance(nullptr);

void MadDecoder::createInstance(std::vector<uint8_t> buffer) {
    if (s_instance == nullptr) {
        s_instance = std::unique_ptr<MadDecoder>(new MadDecoder(std::move(buffer)));
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

MadDecoder::MadDecoder(std::vector<uint8_t> buffer)
    : m_buffer(std::move(buffer))
{
    setupLibmad_();
    setupStreamBuffer_();
}

MadDecoder::~MadDecoder()
{
    tearDownLibmad_();
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

}
