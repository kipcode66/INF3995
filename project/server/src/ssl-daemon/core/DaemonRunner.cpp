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

    std::promise<bool> tasksReadyPromise;
    m_tasksReady = tasksReadyPromise.get_future();
    m_workers.push_back(std::thread(&DaemonRunner::reader_, this, std::ref(clientSession), std::ref(httpServerSocket)));
    m_workers.push_back(std::thread(&DaemonRunner::writer_, this, std::ref(clientSession), std::ref(httpServerSocket)));
    tasksReadyPromise.set_value(true);

    std::for_each(
        m_workers.begin(),
        m_workers.end(),
        [](std::thread& worker) { worker.join(); }
    );
}

void DaemonRunner::reader_(SslSession& clientSession, ClientSocket& httpServerSocket) {
    try {
        m_tasksReady.wait();

        while (true) {
            std::string data = clientSession.read();
            httpServerSocket.write(data);
        }
    }
    catch (const SocketClosedException& e) { }
    catch (const std::exception& e) {
        std::cerr << "C++ exception thrown in reader thread : " << e.what() << std::endl;
    }
    catch (...) {
        killAll_();
        throw;
    }

    killAll_();
}

void DaemonRunner::writer_(SslSession& clientSession, ClientSocket& httpServerSocket) {
    try {
        m_tasksReady.wait();

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
        }
    }
    catch (const SocketClosedException& e) { }
    catch (const std::exception& e) {
        std::cerr << "C++ exception thrown in writer thread : " << e.what() << std::endl;
    }
    catch (...) {
        killAll_();
        throw;
    }

    killAll_();
}

void DaemonRunner::killAll_() {
    std::lock_guard<std::mutex> killLock(m_killMutex);
    std::for_each(
        m_workers.begin(),
        m_workers.end(),
        [](std::thread& worker) {
            std::thread::native_handle_type handle = worker.native_handle();
            if (handle != ::pthread_self() && worker.joinable()) {
                ::pthread_cancel(handle);
            }
        }
    );
}

} // namespace daemon
} // namespace elevation
