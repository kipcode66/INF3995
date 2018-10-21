#include "MadStream.hpp"

namespace elevation {

std::unique_ptr<MadStream> MadStream::s_instance(nullptr);

void MadStream::createInstance(std::vector<uint8_t> buffer) {
    // TODO Throw when instance is already created.
}

MadStream& MadStream::getInstance() {
    // TODO Throw when instance not yet created.
    return *s_instance;
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
