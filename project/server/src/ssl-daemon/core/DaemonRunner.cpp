#include "DaemonRunner.hpp"

#include <iostream>

namespace elevation {
namespace daemon {

DaemonRunner::DaemonRunner(SslSession clientSession, ClientSocket httpServerSocket)
{
    std::thread runnerThread(&DaemonRunner::runner_, this, std::move(clientSession), std::move(httpServerSocket));
    runnerThread.detach();
}

DaemonRunner::~DaemonRunner() { }

void DaemonRunner::runner_(SslSession clientSession, ClientSocket httpServerSocket) {
    // This thread keeps the clientSession. When this thread ends, the clientSession
    // is closed by the SslSession destructor.

    std::thread readerThread(&DaemonRunner::reader_, this, std::ref(clientSession), std::ref(httpServerSocket));
    std::thread writerThread(&DaemonRunner::writer_, this, std::ref(clientSession), std::ref(httpServerSocket));
    readerThread.join();
    writerThread.join();
}

void DaemonRunner::reader_(SslSession& clientSession, ClientSocket& httpServerSocket) {
    std::string gottenData = clientSession.read();
    httpServerSocket << gottenData;
}

void DaemonRunner::writer_(SslSession& clientSession, ClientSocket& httpServerSocket) {
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
