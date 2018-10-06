#ifndef CORE_DAEMON_RUNNER_HPP
#define CORE_DAEMON_RUNNER_HPP

#include "ssl/SslSession.hpp"

#include <thread>

namespace elevation {
namespace daemon {

class DaemonRunner {
public:
    DaemonRunner(SslSession session);
    ~DaemonRunner();

protected:
    void reader_();
    void writer_();

protected:
    SslSession m_session;
    std::thread m_readerThread;
    std::thread m_writerThread;
};

} // namespace daemon
} // namespace elevation

#endif // !CORE_DAEMON_RUNNER_HPP
