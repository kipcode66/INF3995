#define BOOST_TEST_MODULE VolumeChangeEvent
#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include <TestResources.hpp>

#include "mp3/event/VolumeChangeEvent.hpp"

BOOST_TEST_DONT_PRINT_LOG_VALUE(elevation::Mp3Event::EventType);

namespace elevation {

bool areEqual(const std::string& gottenValue, const std::vector<uint8_t>& expectedValue) {
    if (gottenValue.size() != expectedValue.size()) {
        return false;
    }

    for (std::size_t i = 0; i < gottenValue.size(); ++i) {
        std::size_t value    = (uint8_t)gottenValue[i];
        std::size_t expected = (uint8_t)expectedValue[i];
        if (value != expected) {
            return false;
        }
    }
    return true;
}

std::vector<uint8_t> getSignatureAndTypeVector() {
    std::vector<uint8_t> signatureAndTypeVector;
    constexpr std::size_t MASK = 0xFF;
    uint32_t signature = Mp3Event::EVENT_SIGNATURE;
    for (std::size_t i = 0; i < sizeof(Mp3Event::EVENT_SIGNATURE); ++i) {
        signatureAndTypeVector.push_back(signature & MASK);
        signature >>= 8 * sizeof(signatureAndTypeVector[0]);
    }
    uint32_t eventType = static_cast<uint32_t>(Mp3Event::EventType::VOLUME_CHANGE);
    for (std::size_t i = 0; i < sizeof(Mp3Event::EventType::VOLUME_CHANGE); ++i) {
        signatureAndTypeVector.push_back(eventType & MASK);
        eventType >>= 8 * sizeof(signatureAndTypeVector[0]);
    }
    return signatureAndTypeVector;
}

BOOST_AUTO_TEST_CASE(constructionDestruction) {
    VolumeChangeEvent event{42};
    BOOST_TEST(event.getNewVolume() == 42);
    BOOST_TEST(event.getEventType() == Mp3Event::EventType::VOLUME_CHANGE);
}

BOOST_AUTO_TEST_CASE(serialize) {
    struct TestCase {
        uint8_t newVolume;
        std::vector<uint8_t> expectedSerialization;
    };

    std::vector<TestCase> testCases;

    // Signature
    std::vector<uint8_t> volumeChangeSerialization = getSignatureAndTypeVector();

    volumeChangeSerialization.push_back(42);
    testCases.push_back({42 , volumeChangeSerialization});

    for (const auto& testCase : testCases) {
        std::vector<uint8_t> expectedSerialization = testCase.expectedSerialization;
        for (std::size_t i = expectedSerialization.size(); i < Mp3Event::DATA_SIZE; ++i) {
            expectedSerialization.push_back(Mp3Event::PADDING);
        }

        VolumeChangeEvent event{testCase.newVolume};
        BOOST_TEST(areEqual(event.serialize(), expectedSerialization));
    }
}

} // namespace elevation
