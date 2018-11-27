#include "DaemonRunner.hpp"

#include <iostream>
#include <regex>

#include "packet/HttpPacketReader.hpp"

#include <common/os/exception/SocketClosedException.hpp>

namespace elevation {
namespace daemon {

DaemonRunner::DaemonRunner(SslSession clientSession, ClientSocket httpServerSocket)
{
    std::thread runnerThread(&DaemonRunner::runner_, std::move(clientSession), std::move(httpServerSocket));
    runnerThread.detach();
}

DaemonRunner::~DaemonRunner()
{ }

void DaemonRunner::runner_(SslSession clientSession, ClientSocket httpServerSocket) {
    // This thread keeps the clientSession and the socket. When this thread ends, these
    // are closed by their respective destructors.

    std::promise<bool> tasksReadyPromise;
    std::shared_ptr<std::shared_future<bool>> tasksReady(new std::shared_future<bool>(tasksReadyPromise.get_future()));
    std::shared_ptr<std::promise<void>> terminatePromise(new std::promise<void>());
    std::future<void> terminateFuture = terminatePromise->get_future();

    std::shared_ptr<SslSession> clientSessionPtr(new SslSession(std::move(clientSession)));
    std::shared_ptr<ClientSocket> httpServerSocketPtr(new ClientSocket(std::move(httpServerSocket)));
    std::thread serverForwarder = std::thread(&DaemonRunner::forwardToServer_, clientSessionPtr, httpServerSocketPtr, tasksReady, terminatePromise);
    std::thread clientForwarder = std::thread(&DaemonRunner::forwardToClient_, clientSessionPtr, httpServerSocketPtr, tasksReady, terminatePromise);
    tasksReadyPromise.set_value(true);

    try {
        terminateFuture.wait();
        kill_(serverForwarder);
        kill_(clientForwarder);
    }
    catch(const std::exception& e) { }

    try {
        clientForwarder.detach();
    }
    catch (const std::exception& e) { }
    try {
        serverForwarder.detach();
    }
    catch (const std::exception& e) { }
}

void DaemonRunner::forwardToServer_(std::shared_ptr<SslSession> clientSession, std::shared_ptr<ClientSocket> httpServerSocket, std::shared_ptr<std::shared_future<bool>> tasksReady, std::shared_ptr<std::promise<void>> terminate) {
    try {
        tasksReady->wait();

        while (true) {
            std::string data = clientSession->read();
            httpServerSocket->write(data);
        }
    }
    catch (const SocketClosedException& e) { }
    catch (const std::exception& e) {
        std::cerr << "C++ exception thrown in reader thread : " << e.what() << std::endl;
    }
    catch (...) {
        try {
            terminate->set_value();
        }
        catch (std::exception& e) { }
        throw;
    }

    try {
        terminate->set_value();
    }
    catch (std::exception& e) { }
}

void DaemonRunner::forwardToClient_(std::shared_ptr<SslSession> clientSession, std::shared_ptr<ClientSocket> httpServerSocket, std::shared_ptr<std::shared_future<bool>> tasksReady, std::shared_ptr<std::promise<void>> terminate) {
    try {
        tasksReady->wait();

        HttpPacketReader packetReader(httpServerSocket);
        while (true) {
            clientSession->write(packetReader.readPacket());
        }
    }
    catch (const SocketClosedException& e) { }
    catch (const std::exception& e) {
        std::cerr << "C++ exception thrown in writer thread : " << e.what() << std::endl;
    }
    catch (...) {
        try {
            terminate->set_value();
        }
        catch (std::exception& e) { }
        throw;
    }

    try {
        terminate->set_value();
    }
    catch (std::exception& e) { }
}

void DaemonRunner::kill_(std::thread& thread) {
    std::thread::native_handle_type handle = thread.native_handle();
    if (handle != ::pthread_self() && thread.joinable()) {
        ::pthread_cancel(handle);
    }
}

} // namespace daemon
} // namespace elevation
