#define BOOST_TEST_MODULE PendingSongs
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <vector>
#include <atomic>
#include <mutex>

#include <mp3/list/PendingSongs.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

namespace fs = ::std::experimental::filesystem;

class TestFixture {
public:
    explicit TestFixture()
        : m_pendingSongs(
            std::bind(&TestFixture::nextSongGetter, this),
            std::bind(&TestFixture::songRemover, this, std::placeholders::_1)
          )
        , m_nextSong("")
        , m_lastRemovedSong("")
    { }

    fs::path nextSongGetter();
    void songRemover(fs::path song);

protected:
    PendingSongs m_pendingSongs;
    std::mutex m_mutex;
    fs::path m_nextSong;
    fs::path m_lastRemovedSong;
};

fs::path TestFixture::nextSongGetter() {
    std::lock_guard<std::mutex> lock(m_mutex);
    fs::path nextSong = std::move(m_nextSong);
    m_nextSong = "";
    return nextSong;
}

void TestFixture::songRemover(fs::path song) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_lastRemovedSong = song;
}

BOOST_FIXTURE_TEST_CASE(constructionDestruction, TestFixture) {
}

} // namespace elevation
