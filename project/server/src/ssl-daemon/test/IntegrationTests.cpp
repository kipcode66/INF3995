// #define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IntegrationTests
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <signal.h>

#include "communication/ListenerSocket.hpp"
#include "communication/ClientSocket.hpp"
#include "elevation/daemon/Config.hpp"

namespace elevation {
namespace daemon {


void tearDown(__pid_t childId) {
    ::kill(childId, SIGKILL);
}

static std::pair<__pid_t, std::unique_ptr<ListenerSocket>> setup() {
    __pid_t childId = -1;
    try {
        uint16_t serverPortNum = 10080;
        const uint16_t SERVER_PORT_NUM_MAX = 10100;
        std::unique_ptr<ListenerSocket> fakeServer;
        bool serverSetupDone = false;
        try {
            fakeServer = std::unique_ptr<ListenerSocket>(new ListenerSocket(serverPortNum));
            serverSetupDone = true;
        }
        catch (const std::exception& e) {
            ++serverPortNum;
        }
        if (serverPortNum > SERVER_PORT_NUM_MAX) {
            throw std::runtime_error("Could not start fakeServer : no port available.");
        }

        childId = ::vfork();
        if (childId < 0) {
            throw std::runtime_error(std::string("Could not vfork() : ") + ::strerror(errno));
        }
        else if (childId == 0) {
            int error = ::execl(
                SSL_DAEMON_EXECUTABLE,
                SSL_DAEMON_EXECUTABLE,
                "-l",
                "4433",
                "-o",
                std::to_string(serverPortNum).c_str(),
                (char*)NULL
            );
            throw std::runtime_error(std::string("Could not execl() : ") + ::strerror(errno));
        }
        return std::make_pair(childId, std::move(fakeServer));
    }
    catch (...) {
        if (childId > 0) {
            tearDown(childId);
        }
        throw;
    }
}

BOOST_AUTO_TEST_CASE(sanity) {
    auto data = setup();
    __pid_t childPid = data.first;
    std::unique_ptr<ListenerSocket> fakeServer = std::move(data.second);

    try {
        using namespace std::chrono_literals; // For s, ms, us, ns suffixes
        bool connectionEstablished = false;
        uint32_t tries = 0;
        constexpr const uint32_t MAX_TRIES = 30;
        const auto SLEEP_TIME = 100ms;

        while (!connectionEstablished && tries < MAX_TRIES) {
            try {
                ClientSocket client(4433);
                connectionEstablished = true;
            }
            catch (...) {
                std::this_thread::sleep_for(SLEEP_TIME);
            }
            ++tries;
        }
        BOOST_REQUIRE(connectionEstablished);
    }
    catch (...) {
        tearDown(childPid);
        throw;
    }

    tearDown(childPid);
}

} // namespace daemon
} // namespace elevation
