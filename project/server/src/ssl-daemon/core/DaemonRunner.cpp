#include "DaemonRunner.hpp"

#include <iostream>
#include <regex>

#include "packet/HttpPacketReader.hpp"

#include <common/os/exception/SocketClosedException.hpp>

namespace elevation {
namespace daemon {

DaemonRunner::DaemonRunner(SslSession clientSession, ClientSocket httpServerSocket)
{
    std::thread runnerThread(&DaemonRunner::runner_, this, std::move(clientSession), std::move(httpServerSocket));
    runnerThread.detach();
}

DaemonRunner::~DaemonRunner() {
    killAll_();
    try {
        m_clientForwarder.detach();
    }
    catch (const std::exception& e) { }

    try {
        m_serverForwarder.detach();
    }
    catch (const std::exception& e) { }
}

void DaemonRunner::runner_(SslSession clientSession, ClientSocket httpServerSocket) {
    // This thread keeps the clientSession and the socket. When this thread ends, these
    // are closed by their respective destructors.

    std::promise<bool> tasksReadyPromise;
    m_tasksReady = tasksReadyPromise.get_future();
    m_serverForwarder = std::thread(&DaemonRunner::forwardToServer_, this, std::ref(clientSession), std::ref(httpServerSocket));
    m_clientForwarder = std::thread(&DaemonRunner::forwardToClient_, this, std::ref(clientSession), std::ref(httpServerSocket));
    tasksReadyPromise.set_value(true);
}

void DaemonRunner::forwardToServer_(SslSession& clientSession, ClientSocket& httpServerSocket) {
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

void DaemonRunner::forwardToClient_(SslSession& clientSession, ClientSocket& httpServerSocket) {
    try {
        m_tasksReady.wait();

        HttpPacketReader packetReader(httpServerSocket);
        while (true) {
            clientSession.write(packetReader.readPacket());
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
    std::thread::native_handle_type clientHandle = m_clientForwarder.native_handle();
    if (clientHandle != ::pthread_self() && m_clientForwarder.joinable()) {
        ::pthread_cancel(clientHandle);
    }
    std::thread::native_handle_type serverHandle = m_serverForwarder.native_handle();
    if (serverHandle != ::pthread_self() && m_serverForwarder.joinable()) {
        ::pthread_cancel(serverHandle);
    }
}

} // namespace daemon
} // namespace elevation
