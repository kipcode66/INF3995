#define BOOST_TEST_MODULE Mp3Event
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <iostream>

#include <TestResources.hpp>

#include "mp3/event/Mp3Event.hpp"

namespace elevation {

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

} // namespace elevation
