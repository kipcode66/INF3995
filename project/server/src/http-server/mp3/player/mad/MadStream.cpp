#include "MadStream.hpp"

#include <stdexcept>

namespace elevation {

std::unique_ptr<MadStream> MadStream::s_instance(nullptr);

void MadStream::createInstance(std::vector<uint8_t> buffer) {
    if (s_instance == nullptr) {
        s_instance = std::unique_ptr<MadStream>(new MadStream(std::move(buffer)));
    }
    else {
        throw std::runtime_error("Cannot create MadStream instance : Already exists");
    }
}

MadStream& MadStream::getInstance() {
    if (s_instance != nullptr) {
        return *s_instance;
    }
    else {
        throw std::runtime_error("Cannot get MadStream instance : Not yet created");
    }
}

MadStream::MadStream(std::vector<uint8_t> buffer)
    : m_buffer(std::move(buffer))
{
    setupLibmad_();
    setupStreamBuffer_();
}

MadStream::~MadStream()
{
    tearDownLibmad_();
}

void MadStream::setupLibmad_() {
    mad_stream_init(&m_stream);
    mad_synth_init (&m_synth );
    mad_frame_init (&m_frame );
}

void MadStream::setupStreamBuffer_() {
    mad_stream_buffer(&m_stream, &m_buffer.at(0), m_buffer.size() * sizeof(m_buffer.at(0)));
}

void MadStream::tearDownLibmad_() {
    mad_synth_finish (&m_synth );
    mad_frame_finish (&m_frame );
    mad_stream_finish(&m_stream);
}

}
