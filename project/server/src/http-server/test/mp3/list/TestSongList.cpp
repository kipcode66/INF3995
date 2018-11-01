#define BOOST_TEST_MODULE SongList
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <vector>

#include <mp3/list/SongList.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

class TestFixture {
public:
    static constexpr std::size_t LIST_SIZE = 3;

public:
    explicit TestFixture()
        : m_songList(LIST_SIZE)
    { }

    void fillup() {
        for (std::size_t i = 0; i < LIST_SIZE; ++i) {
            BOOST_CHECK_NO_THROW(m_songList.push(TEST_MP3_PATH));
        }
    }

    void empty() {
        BOOST_CHECK_NO_THROW(m_songList.remove(TEST_MP3_PATH));
    }

protected:
    SongList m_songList;
};

BOOST_FIXTURE_TEST_CASE(constructionDestruction, TestFixture) {
}

BOOST_FIXTURE_TEST_CASE(pushSong, TestFixture) {
    fillup();
}

BOOST_FIXTURE_TEST_CASE(pushTooManySongs, TestFixture) {
    fillup();
    BOOST_CHECK_THROW(m_songList.push(TEST_MP3_PATH), std::exception);
}

BOOST_FIXTURE_TEST_CASE(remove, TestFixture) {
    fillup();
    empty();
}

BOOST_FIXTURE_TEST_CASE(removeTooManySongs, TestFixture) {
    BOOST_CHECK_THROW(m_songList.remove(TEST_MP3_PATH), std::exception);
    fillup();
    empty();
    BOOST_CHECK_THROW(m_songList.remove(TEST_MP3_PATH), std::exception);
}

BOOST_FIXTURE_TEST_CASE(swapSongs, TestFixture) {
    struct TestCase {
        std::size_t songA;
        std::size_t songB;
        std::list<std::experimental::filesystem::path> expectedResult;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({0, 0, {TEST_MP3_PATH, TIPPERARY_PATH, TIPPERARY_SHORT_PATH}});
    testCases.push_back({1, 0, {TIPPERARY_PATH, TEST_MP3_PATH, TIPPERARY_SHORT_PATH}});
    testCases.push_back({0, 1, {TIPPERARY_PATH, TEST_MP3_PATH, TIPPERARY_SHORT_PATH}});
    testCases.push_back({1, 2, {TEST_MP3_PATH, TIPPERARY_SHORT_PATH, TIPPERARY_PATH}});
    testCases.push_back({2, 0, {TIPPERARY_SHORT_PATH, TIPPERARY_PATH, TEST_MP3_PATH}});

    for (const auto& testCase : testCases) {
        for (std::size_t i = 0; i < LIST_SIZE; ++i) {
            std::string mp3;
            switch (i % 3) {
            case 0: mp3 = TEST_MP3_PATH       ; break;
            case 1: mp3 = TIPPERARY_PATH      ; break;
            case 2: mp3 = TIPPERARY_SHORT_PATH; break;
            }
            BOOST_CHECK_NO_THROW(m_songList.push(mp3));
        }
        BOOST_CHECK_NO_THROW(m_songList.swap(testCase.songA, testCase.songB));
        BOOST_TEST(m_songList.getSongList() == testCase.expectedResult);
        BOOST_CHECK_NO_THROW(m_songList.remove(TEST_MP3_PATH));
        BOOST_CHECK_NO_THROW(m_songList.remove(TIPPERARY_PATH));
        BOOST_CHECK_NO_THROW(m_songList.remove(TIPPERARY_SHORT_PATH));
    }
}

BOOST_FIXTURE_TEST_CASE(swapNonexistantSongs, TestFixture) {
    struct TestCase {
        std::size_t listSize;
        std::size_t songA;
        std::size_t songB;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({0, 0, 0});
    testCases.push_back({0, 1, 0});
    testCases.push_back({0, 0, 1});
    testCases.push_back({1, 1, 2});
    testCases.push_back({1, 2, 1});
    testCases.push_back({1, 2, 0});
    testCases.push_back({2, 2, 3});
    testCases.push_back({2, 3, 0});

    for (const auto& testCase : testCases) {
        for (std::size_t i = 0; i < testCase.listSize; ++i) {
            std::string mp3;
            switch (i % 3) {
            case 0: mp3 = TEST_MP3_PATH       ; break;
            case 1: mp3 = TIPPERARY_PATH      ; break;
            case 2: mp3 = TIPPERARY_SHORT_PATH; break;
            }
            BOOST_CHECK_NO_THROW(m_songList.push(mp3));
        }
        BOOST_CHECK_THROW(m_songList.swap(testCase.songA, testCase.songB), std::exception);
        try {m_songList.remove(TEST_MP3_PATH);       } catch (std::out_of_range& e) { }
        try {m_songList.remove(TIPPERARY_PATH);      } catch (std::out_of_range& e) { }
        try {m_songList.remove(TIPPERARY_SHORT_PATH);} catch (std::out_of_range& e) { }
    }
}

} // namespace elevation
