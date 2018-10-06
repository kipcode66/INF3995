#include "DaemonRunner.hpp"

#include <iostream>

namespace elevation {
namespace daemon {

DaemonRunner::DaemonRunner(SslSession clientSession)
{
    std::thread runnerThread(&DaemonRunner::runner_, this, std::move(clientSession));
    runnerThread.detach();
}

DaemonRunner::~DaemonRunner() { }

void DaemonRunner::runner_(SslSession clientSession) {
    // This thread keeps the clientSession. When this thread ends, the clientSession
    // is closed by the SslSession destructor.

    std::thread readerThread(&DaemonRunner::reader_, this, std::ref(clientSession));
    std::thread writerThread(&DaemonRunner::writer_, this, std::ref(clientSession));
    readerThread.join();
    writerThread.join();
}

void DaemonRunner::reader_(SslSession& clientSession) {
    std::cout << clientSession.read() << std::endl;
}

void DaemonRunner::writer_(SslSession& clientSession) {
    clientSession.write(
        "HTTP/1.1 404 Not Found\n"
        "Connection: Keep-Alive\n"
        "Content-Length: 13\n"
        "\n"
        "404 not found"
    );
}

} // namespace daemon
} // namespace elevation
