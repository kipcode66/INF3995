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
{ }

MadStream::~MadStream() { }

}
