#define BOOST_TEST_MODULE PulseDevice
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <thread>
#include <cstring>

#include <mp3/player/pulse/PulseDevice.hpp>
#include <os/SharedFileMemory.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

class TestFixture {
public:
    PulseDevice m_pulse;
};

BOOST_FIXTURE_TEST_CASE(constructionDestruction, TestFixture) {
}

BOOST_FIXTURE_TEST_CASE(moveConstructor, TestFixture) {
    PulseDevice movedPulse(std::move(m_pulse));
}

BOOST_FIXTURE_TEST_CASE(moveAssignment, TestFixture) {
    PulseDevice movedPulse;
    movedPulse = std::move(m_pulse);
}

BOOST_FIXTURE_TEST_CASE(playSanityCheck, TestFixture) {
    SharedFileMemory fileMemory(TIPPERARY_SHORT_WAV_PATH);
    constexpr std::size_t MAX_FRAME_LENGTH = 2 << 10;
    std::vector<uint8_t> buffer;
    std::size_t currentPosition = 0;
    while (currentPosition < fileMemory.size() - MAX_FRAME_LENGTH) {
        std::size_t frameLength = std::min(fileMemory.size() - currentPosition, MAX_FRAME_LENGTH);
        buffer.resize(0);
        std::copy(
            &fileMemory[currentPosition],
            &fileMemory[currentPosition + frameLength - 1] + 1,
            std::back_inserter(buffer)
        );
        m_pulse.play(buffer);
        currentPosition += frameLength;
    }
}

} // namespace elevation
