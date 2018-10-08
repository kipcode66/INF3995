#include "DaemonRunner.hpp"

#include <iostream>
#include <regex>

#include "communication/SocketClosedException.hpp"

namespace elevation {
namespace daemon {

DaemonRunner::DaemonRunner(SslSession clientSession, ClientSocket httpServerSocket)
{
    std::thread runnerThread(&DaemonRunner::runner_, this, std::move(clientSession), std::move(httpServerSocket));
    runnerThread.detach();
}

DaemonRunner::~DaemonRunner() { }

void DaemonRunner::runner_(SslSession clientSession, ClientSocket httpServerSocket) {
    // This thread keeps the clientSession and the socket. When this thread ends, these
    // are closed by their respective destructors.

    std::thread readerThread(&DaemonRunner::reader_, this, std::ref(clientSession), std::ref(httpServerSocket));
    std::thread writerThread(&DaemonRunner::writer_, this, std::ref(clientSession), std::ref(httpServerSocket));
    readerThread.join();
    writerThread.join();
}

void DaemonRunner::reader_(SslSession& clientSession, ClientSocket& httpServerSocket) {
    try {
        std::string data = clientSession.read();
        httpServerSocket << data;
    }
    catch (const SocketClosedException& e) {
        std::cerr << "Reader thread : Socket closed." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "C++ exception thrown in reader thread : " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception thrown in reader thread." << std::endl;
    }
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
