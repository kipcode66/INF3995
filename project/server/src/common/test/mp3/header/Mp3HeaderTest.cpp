#include "mp3/header/Mp3Header.hpp"

#define BOOST_TEST_MODULE Mp3Header
#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include <TestConfiguration.hpp>

namespace elevation {

BOOST_AUTO_TEST_CASE(mp3HeaderFailure) {
    std::vector<std::string> testInput;

    testInput.push_back(TEST_TEXT_PATH);
    testInput.push_back("does-not-exist.mp3");
    testInput.push_back(TEST_MP3_AS_TXT_PATH);
    testInput.push_back(TEST_TXT_AS_MP3_PATH);

    for (const auto& testCase : testInput) {
        BOOST_CHECK_THROW(Mp3Header mp3Header(testCase), std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(mp3HeaderSucces) {
    Mp3Header mp3TestHeader = Mp3Header(TEST_MP3_PATH);

    BOOST_CHECK_EQUAL(mp3TestHeader.getAlbum(), "Plain Pat & Emile Presents A KiD Named CuDi");
    BOOST_CHECK_EQUAL(mp3TestHeader.getAlbum(), "Plain Pat & Emile Presents A KiD Named CuDi");
    BOOST_CHECK_EQUAL(mp3TestHeader.getArtist(), "Kid Cudi");
    BOOST_CHECK_EQUAL(mp3TestHeader.getDuration().getMinutes(), 0);
    BOOST_CHECK_EQUAL(mp3TestHeader.getDuration().getSeconds(), 50);
    BOOST_CHECK_EQUAL(mp3TestHeader.getGenre(), "Hip-Hop");
    BOOST_CHECK_EQUAL(mp3TestHeader.getTitle(), "Intro");
    BOOST_CHECK_EQUAL(mp3TestHeader.getTrack(), 1);
    BOOST_CHECK_EQUAL(mp3TestHeader.getYear(), 2008);
}

} // namespace elevation
