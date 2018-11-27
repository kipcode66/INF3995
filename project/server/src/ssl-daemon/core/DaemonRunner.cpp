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
    std::lock_guard<std::mutex> lock(m_workersMutex);
    std::for_each(m_workers.begin(), m_workers.end(), [](std::thread& worker) {
        try {
            worker.detach();
        }
        catch (const std::exception& e) { }
    });
}

void DaemonRunner::runner_(SslSession clientSession, ClientSocket httpServerSocket) {
    // This thread keeps the clientSession and the socket. When this thread ends, these
    // are closed by their respective destructors.

    std::promise<bool> tasksReadyPromise;
    m_tasksReady = tasksReadyPromise.get_future();

    {
        std::lock_guard<std::mutex> lock(m_workersMutex);
        m_workers.push_back(std::thread(&DaemonRunner::forwardToServer_, this, std::ref(clientSession), std::ref(httpServerSocket)));
        m_workers.push_back(std::thread(&DaemonRunner::forwardToClient_, this, std::ref(clientSession), std::ref(httpServerSocket)));
        // m_workersMutex get unlocked here
    }
    tasksReadyPromise.set_value(true);


    {
        std::lock_guard<std::mutex> lock(m_workersMutex);
        std::for_each(
            m_workers.begin(),
            m_workers.end(),
            [](std::thread& worker) {
                try {
                    worker.join();
                }
                catch (const std::exception& e) { }
            }
        );
        // m_workersMutex get unlocked here
    }
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
    std::lock_guard<std::mutex> lock(m_workersMutex);
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
