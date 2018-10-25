#define BOOST_TEST_MODULE Mp3Player
#include <boost/test/unit_test.hpp>

#include <chrono>

#include <mp3/player/Mp3Player.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

class TestFixture {
public:
    Mp3Player m_player;
};

BOOST_FIXTURE_TEST_CASE(constructionDestruction, TestFixture) {
}

BOOST_FIXTURE_TEST_CASE(moveConstructor, TestFixture) {
    Mp3Player moved(std::move(m_player));
}

unsigned int TIMEOUT_SECONDS = 10;
BOOST_AUTO_TEST_CASE(startPlaying_and_waitUntilSongFinished, *boost::unit_test::timeout(TIMEOUT_SECONDS)) {
    using namespace std::chrono_literals;
    const auto MINIMUM_DURATION = TIPPERARY_SHORT_DURATION_SECONDS / 1.2;
    const auto MAXIMUM_DURATION = TIPPERARY_SHORT_DURATION_SECONDS * 1.2;

    Mp3Player player;
    BOOST_CHECK_NO_THROW(player.waitUntilSongFinished());

    const unsigned int NUM_TRIES = 2;
    for (unsigned int i = 0; i < NUM_TRIES; ++i) {
        auto startTime = std::chrono::system_clock::now();
        BOOST_CHECK_NO_THROW(player.startPlaying(TIPPERARY_SHORT_PATH));
        BOOST_CHECK_NO_THROW(player.waitUntilSongFinished());
        BOOST_CHECK_NO_THROW(player.waitUntilSongFinished());
        auto endTime = std::chrono::system_clock::now();

        auto duration(std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime));
        BOOST_TEST(duration.count() > MINIMUM_DURATION.count());
        BOOST_TEST(duration.count() < MAXIMUM_DURATION.count());
    }
}

} // namespace elevation
