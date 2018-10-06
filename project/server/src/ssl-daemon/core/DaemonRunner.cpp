#include "DaemonRunner.hpp"

#include <iostream>

namespace elevation {
namespace daemon {

DaemonRunner::DaemonRunner(SslSession session)
    : m_session(std::move(session))
{
}

DaemonRunner::~DaemonRunner() { }

void DaemonRunner::reader_() {
}

void DaemonRunner::writer_() {
}

} // namespace daemon
} // namespace elevation
