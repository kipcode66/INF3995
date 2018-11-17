#define BOOST_TEST_MODULE Mp3Event
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <iostream>

#include <TestResources.hpp>

#include "mp3/event/Mp3Event.hpp"

namespace elevation {

bool areEqual(const std::string& gottenValue, const std::vector<uint8_t>& expectedValue) {
    if (gottenValue.size() != expectedValue.size()) {
        std::cout << std::hex << "Got vector of size" << gottenValue.size() << "' instead of '" << expectedValue.size() << "'" << std::endl;
        return false;
    }

    for (std::size_t i = 0; i < gottenValue.size(); ++i) {
        std::size_t value    = (uint8_t)gottenValue[i];
        std::size_t expected = (uint8_t)expectedValue[i];
        if (value != expected) {
            std::cout << std::hex << "Got '" << value << "' instead of '" << expected << "' at position " << std::dec << i << std::endl;
            return false;
        }
    }
    return true;
}

class TestMp3Event : public Mp3Event {
public:
    explicit TestMp3Event(EventType eventType = EventType::VOLUME_CHANGE) : Mp3Event(eventType) { }
    virtual ~TestMp3Event() = default;

    virtual std::string serialize() const override;
};

std::string TestMp3Event::serialize() const {
    return Mp3Event::serialize();
}

BOOST_AUTO_TEST_CASE(constructionDestruction) {
    volatile TestMp3Event testEvent;
}

BOOST_AUTO_TEST_CASE(constructWithInvalidType) {
    struct TestCase {
        Mp3Event::EventType eventType;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({Mp3Event::EventType::NONE});
    testCases.push_back({Mp3Event::EventType::NUM_EVENTS});
    testCases.push_back({static_cast<Mp3Event::EventType>(static_cast<uint32_t>(Mp3Event::EventType::NUM_EVENTS) + 1)});

    for (const auto& testCase : testCases) {
        BOOST_CHECK_THROW(TestMp3Event{testCase.eventType}, std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(serializeElement) {
    struct TestCase {
        Mp3Event::EventType eventType;
        uint8_t byte;
        uint16_t halfWord;
        uint32_t word;
        uint64_t doubleWord;
        std::vector<uint8_t> expectedEventTypeSerialization;
        std::vector<uint8_t> expectedByteSerialization;
        std::vector<uint8_t> expectedHalfWordSerialization;
        std::vector<uint8_t> expectedWordSerialization;
        std::vector<uint8_t> expectedDoubleWordSerialization;
    };

    std::vector<TestCase> testCases;
    testCases.push_back({Mp3Event::EventType::NONE         , 211, 32537, 4294000000, 0xF0A5A5A5A5A5A5A5ULL, {0x00, 0x00}, {211}, {0x19, 0x7F}, {0x80, 0x3D, 0xF1, 0xFF}, {0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xF0}});
    testCases.push_back({Mp3Event::EventType::VOLUME_CHANGE, 255, 65535, 4294967295, 0xFFFFFFFFFFFFFFFFULL, {0x01, 0x00}, {255}, {0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}});
    testCases.push_back({Mp3Event::EventType::VOLUME_CHANGE, 0  ,     0,          0, 0x0000000000000000ULL, {0x01, 0x00}, {0  }, {0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}});

    for (const auto& testCase : testCases) {
        BOOST_TEST(areEqual(Mp3Event::serializeElement(testCase.eventType) , testCase.expectedEventTypeSerialization ));
        BOOST_TEST(areEqual(Mp3Event::serializeElement(testCase.byte)      , testCase.expectedByteSerialization      ));
        BOOST_TEST(areEqual(Mp3Event::serializeElement(testCase.halfWord)  , testCase.expectedHalfWordSerialization  ));
        BOOST_TEST(areEqual(Mp3Event::serializeElement(testCase.word)      , testCase.expectedWordSerialization      ));
        BOOST_TEST(areEqual(Mp3Event::serializeElement(testCase.doubleWord), testCase.expectedDoubleWordSerialization));
    }
}

} // namespace elevation
