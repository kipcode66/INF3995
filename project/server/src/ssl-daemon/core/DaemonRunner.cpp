#include "DaemonRunner.hpp"

#include <iostream>
#include <regex>

#include "communication/exception/SocketClosedException.hpp"

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
        while (true) {
            std::string data = clientSession.read();
            httpServerSocket << data;
            std::cout << "Packet read done" << std::endl;
        }
    }
    catch (const SocketClosedException& e) {
        std::cerr << "Reader thread : Socket closed." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "C++ exception thrown in reader thread : " << e.what() << std::endl;
        // TODO Close socket cleanly.
    }
    catch (...) {
        std::cerr << "Unknown exception thrown in reader thread." << std::endl;
        // TODO Close socket cleanly.
    }

    std::cout << "Reader done." << std::endl;
}

void DaemonRunner::writer_(SslSession& clientSession, ClientSocket& httpServerSocket) {
    try {
        while (true) {
            const std::regex HTTP_HEADER_END_REGEX("^\r?\n$");
            const std::regex HTTP_CONTENT_SIZE_REGEX("^Content-Length:\\s*(\\d+)", std::regex_constants::icase);
            const std::size_t HTTP_CONTENT_SIZE_RESULT_GROUP_ID = 1;

            std::size_t httpBodySize = 0;
            std::string line = httpServerSocket.readLine();
            while (!std::regex_search(line, HTTP_HEADER_END_REGEX)) {
                std::smatch matchResult;
                if (std::regex_search(line, matchResult, HTTP_CONTENT_SIZE_REGEX)) {
                    httpBodySize = std::stoi(matchResult[HTTP_CONTENT_SIZE_RESULT_GROUP_ID]);
                }

                clientSession.write(line);
                line = httpServerSocket.readLine();
            }
            clientSession.write(line);
            std::string data = httpServerSocket.read(httpBodySize);
            clientSession.write(data);
            std::cout << "Packet write done" << std::endl;
        }
    }
    catch (const SocketClosedException& e) {
        std::cout << "Writer thread : Socket closed." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "C++ exception thrown in writer thread : " << e.what() << std::endl;
        // TODO Close socket cleanly.
    }
    catch (...) {
        std::cerr << "Unknown exception thrown in writer thread." << std::endl;
        // TODO Close socket cleanly.
    }

    std::cout << "Writer done." << std::endl;
}

} // namespace daemon
} // namespace elevation
