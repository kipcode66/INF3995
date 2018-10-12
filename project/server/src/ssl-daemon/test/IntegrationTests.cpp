// #define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IntegrationTests
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <functional>
#include <signal.h>

#include "communication/ListenerSocket.hpp"
#include "communication/ClientSocket.hpp"
#include "elevation/daemon/Config.hpp"

namespace elevation {
namespace daemon {

// ============================================================================
// =                              TEST FIXTURE                                =
// ============================================================================

/**
 * @brief We setup a test class ("test fixture") rather than using BOOST_AUTO_TEST_CASE
 * because we don't want to have to do the setup and the tearing down
 * inside the test function. That way, the test cases will have access to this
 * class's attributes without having to worry about setting them up.
 */
class IntegrationTestFixture {

public:
    static const __pid_t NO_PID = -1;
    static const uint16_t NO_PORT = 0;
    static constexpr const auto SERVER_PORT_RANGE =
        std::pair<uint16_t, uint16_t>(10080, 10100);
    static const uint16_t CLIENT_PORT = 10443;

public:
    explicit IntegrationTestFixture();
    virtual ~IntegrationTestFixture();
    ClientSocket connectToDaemon();

private:
    void setupFakeServer_();
    void setupDaemon_();

protected:
    __pid_t m_childPid;
    std::unique_ptr<ListenerSocket> m_fakeServer;

};

IntegrationTestFixture::IntegrationTestFixture()
    : m_childPid(NO_PID)
    , m_fakeServer(nullptr)
{
    setupFakeServer_();
    setupDaemon_();
}

IntegrationTestFixture::~IntegrationTestFixture()
{
    if (m_childPid != NO_PID) {
        using namespace std::chrono_literals; // For s, ms, us, ns suffixes

        ::kill(m_childPid, SIGTERM);
        std::this_thread::sleep_for(100ms);
        ::kill(m_childPid, SIGKILL);
    }
}

ClientSocket IntegrationTestFixture::connectToDaemon() {
    using namespace std::chrono_literals; // For s, ms, us, ns suffixes
    constexpr const uint32_t MAX_TRIES = 30;
    const auto SLEEP_TIME = 100ms;

    std::unique_ptr<ClientSocket> client;
    bool connectionEstablished = false;
    uint32_t tries = 0;
    while (!connectionEstablished && tries < MAX_TRIES) {
        try {
            client = std::unique_ptr<ClientSocket>(new ClientSocket(CLIENT_PORT));
            connectionEstablished = true;
        }
        catch (const std::exception& e) {
            std::this_thread::sleep_for(SLEEP_TIME);
        }
        ++tries;
    }
    if (!connectionEstablished) {
        throw std::runtime_error("Could not connect to daemon");
    }

    return std::move(*client);
}

void IntegrationTestFixture::setupFakeServer_() {
    uint16_t serverPortNum = SERVER_PORT_RANGE.first;
    std::unique_ptr<ListenerSocket> fakeServer;
    bool serverSetupDone = false;
    while (!serverSetupDone && serverPortNum <= SERVER_PORT_RANGE.second) {
        try {
            fakeServer = std::unique_ptr<ListenerSocket>(new ListenerSocket(serverPortNum));
            serverSetupDone = true;
        }
        catch (const std::exception& e) {
            if (serverPortNum == SERVER_PORT_RANGE.second) {
                throw;
            }
            else {
                ++serverPortNum;
            }
        }
    }
    m_fakeServer = std::move(fakeServer);
}

void IntegrationTestFixture::setupDaemon_() {
    __pid_t childPid = ::vfork();
    if (childPid < 0) {
        throw std::runtime_error(std::string("Could not vfork() : ") + ::strerror(errno));
    }
    else if (childPid == 0) {
        ::execl(
            SSL_DAEMON_EXECUTABLE,
            SSL_DAEMON_EXECUTABLE,
            "-l",
            std::to_string(CLIENT_PORT).c_str(),
            "-o",
            std::to_string(m_fakeServer->getPort()).c_str(),
            (char*)NULL
        );
        throw std::runtime_error(std::string("Child could not execl() : ") + ::strerror(errno));
    }
    m_childPid = childPid;
}

// ============================================================================
// =                                TESTS CASES                               =
// ============================================================================

BOOST_FIXTURE_TEST_CASE(connectionTest, IntegrationTestFixture) { // Makes the test case a class that is a subclass of IntegrationTestFixture
    connectToDaemon(); // Boost will make the test fail and print a message if this throws an exception
}

// We will not implement a "real" integration test, because that would require a
// working SSL client, which would probably take us 15-20 hours to complete.
// However, we added such a manual test.

} // namespace daemon
} // namespace elevation
