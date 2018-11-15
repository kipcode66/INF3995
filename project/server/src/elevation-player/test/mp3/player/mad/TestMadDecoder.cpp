#define BOOST_TEST_MODULE MadDecoder
#include <boost/test/unit_test.hpp>

#include <mp3/player/mad/MadDecoder.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

class TestFixture {
public:
    TestFixture()
        : m_decoder(SharedFileMemory(std::string(TIPPERARY_SHORT_PATH)), MadAudioFormatter())
    { }

    MadDecoder m_decoder;
};

BOOST_FIXTURE_TEST_CASE(constructionDestruction, TestFixture) {
    BOOST_CHECK(!m_decoder.isDone());
}

BOOST_FIXTURE_TEST_CASE(moveConstructor, TestFixture) {
    MadDecoder movedDecoder(std::move(m_decoder));
    BOOST_TEST(m_decoder.isDone());
    BOOST_TEST(!movedDecoder.isDone());
}

BOOST_FIXTURE_TEST_CASE(moveAssignment, TestFixture) {
    MadDecoder movedDecoder(std::move(SharedFileMemory(std::string(TIPPERARY_SHORT_PATH))), MadAudioFormatter());
    movedDecoder = std::move(m_decoder);
    BOOST_TEST(m_decoder.isDone());
    BOOST_TEST(!movedDecoder.isDone());
}

BOOST_FIXTURE_TEST_CASE(decodingSanityCheck, TestFixture) {
    constexpr std::size_t MAX_FRAMES = 1 << 20;
    std::size_t numDecodedFrames = 0;
    while (numDecodedFrames < MAX_FRAMES && !m_decoder.isDone()) {
        m_decoder.decodeNextFrame();
        ++numDecodedFrames;
    }
    BOOST_TEST(numDecodedFrames < MAX_FRAMES);
}

} // namespace elevation
