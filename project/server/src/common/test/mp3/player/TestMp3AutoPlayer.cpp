#define BOOST_TEST_MODULE Mp3AutoPlayer
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <vector>
#include <atomic>
#include <mutex>

#include <mp3/player/Mp3AutoPlayer.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

namespace fs = ::std::experimental::filesystem;

class TestFixture {
public:
    static constexpr const char* REMOVED_SONG_INITIAL_VALUE =
        "/THIS/PATH/IS/AN/INVALID/PATH/IF/IT/APPEARS/THEN/IT/MEANS/A/UNIT/TEST/FAILED";
public:
    explicit TestFixture()
        : m_mp3AutoPlayer(
            std::bind(&TestFixture::nextSongGetter, this),
            std::bind(&TestFixture::songRemover, this, std::placeholders::_1)
          )
        , m_nextSong(Mp3AutoPlayer::NO_SONG)
        , m_lastRemovedSong(REMOVED_SONG_INITIAL_VALUE)
    { }

    fs::path nextSongGetter();
    void songRemover(fs::path song);

public:
    Mp3AutoPlayer m_mp3AutoPlayer;
    std::mutex m_mutex;
    fs::path m_nextSong;
    fs::path m_lastRemovedSong;
};

fs::path TestFixture::nextSongGetter() {
    std::lock_guard<std::mutex> lock(m_mutex);
    fs::path nextSong = m_nextSong;
    m_nextSong = Mp3AutoPlayer::NO_SONG;
    return nextSong;
}

void TestFixture::songRemover(fs::path song) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_lastRemovedSong = song;
}

BOOST_FIXTURE_TEST_CASE(constructionDestruction, TestFixture) {
}

#define TIMEOUT_SECONDS 5 // Have to use BOOST_AUTO_TEST_CASE, because BOOST_FIXTURE_TEST_CASE does not have timeout.
BOOST_AUTO_TEST_CASE(destructionIsOkEvenIfPlaying, *boost::unit_test::timeout(TIMEOUT_SECONDS)) {
    TestFixture fixture;
    {
        std::lock_guard<std::mutex> lock(fixture.m_mutex);
        fixture.m_nextSong = TIPPERARY_PATH;
    }
    fixture.m_mp3AutoPlayer.waitUntilSongStarted();
}
#undef TIMEOUT_SECONDS

#define TIMEOUT_SECONDS 5 // Have to use BOOST_AUTO_TEST_CASE, because BOOST_FIXTURE_TEST_CASE does not have timeout.
BOOST_AUTO_TEST_CASE(stopPlaying, *boost::unit_test::timeout(TIMEOUT_SECONDS)) {
    TestFixture fixture;
    {
        std::lock_guard<std::mutex> lock(fixture.m_mutex);
        fixture.m_nextSong = TIPPERARY_PATH;
    }
    fixture.m_mp3AutoPlayer.waitUntilSongStarted();
    BOOST_TEST(fixture.m_nextSong == Mp3AutoPlayer::NO_SONG);
    BOOST_TEST(fixture.m_lastRemovedSong == TestFixture::REMOVED_SONG_INITIAL_VALUE);
    fixture.m_mp3AutoPlayer.stopSong();
    BOOST_TEST(fixture.m_nextSong == Mp3AutoPlayer::NO_SONG);
    BOOST_TEST(fixture.m_lastRemovedSong == TIPPERARY_PATH);
}
#undef TIMEOUT_SECONDS

#define TIMEOUT_SECONDS 5 // Have to use BOOST_AUTO_TEST_CASE, because BOOST_FIXTURE_TEST_CASE does not have timeout.
BOOST_AUTO_TEST_CASE(callsCallbacksCorrectly, *boost::unit_test::timeout(TIMEOUT_SECONDS)) {
    TestFixture fixture;
    {
        std::lock_guard<std::mutex> lock(fixture.m_mutex);
        fixture.m_nextSong = TIPPERARY_SHORT_PATH;
    }
    fixture.m_mp3AutoPlayer.waitUntilSongStarted();
    BOOST_TEST(fixture.m_nextSong == Mp3AutoPlayer::NO_SONG);
    BOOST_TEST(fixture.m_lastRemovedSong == TestFixture::REMOVED_SONG_INITIAL_VALUE);
    fixture.m_mp3AutoPlayer.waitUntilSongFinished();
    BOOST_TEST(fixture.m_nextSong == Mp3AutoPlayer::NO_SONG);
    BOOST_TEST(fixture.m_lastRemovedSong == TIPPERARY_SHORT_PATH);
}
#undef TIMEOUT_SECONDS

} // namespace elevation
