#define BOOST_TEST_MODULE SharedFileMemory
#include <boost/test/unit_test.hpp>

#include <chrono>

#include <os/SharedFileMemory.hpp>

#include <TestConfiguration.hpp>

namespace elevation {

BOOST_AUTO_TEST_CASE(constructionDestruction) {
    std::vector<std::string> testCases;
    testCases.push_back(TIPPERARY_SHORT_PATH);
    testCases.push_back(TIPPERARY_SHORT_WAV_PATH);
    testCases.push_back(TEST_MP3_AS_TXT_PATH);
    testCases.push_back(TIPPERARY_PATH);
    for (const auto& testCase : testCases) {
        SharedFileMemory fileMemory(testCase);
        BOOST_TEST(fileMemory.size() > 0);
        BOOST_TEST(fileMemory.size() < (1 << 30));
    }
}

BOOST_AUTO_TEST_CASE(moveConstructor) {
    SharedFileMemory original(TEST_TEXT_PATH);
    std::size_t fileSize = original.size();
    SharedFileMemory moved(std::move(original));
    BOOST_TEST(moved.size() == fileSize);
    BOOST_TEST(original.size() == 0);
    BOOST_CHECK_NO_THROW(moved[fileSize - 1]);
    BOOST_CHECK_THROW(original[0], std::exception);
}

BOOST_AUTO_TEST_CASE(moveAssignment) {
    SharedFileMemory original(TEST_TEXT_PATH);
    SharedFileMemory moved(TIPPERARY_PATH);
    std::size_t fileSize = original.size();
    moved = std::move(original);
    BOOST_TEST(moved.size() == fileSize);
    BOOST_TEST(original.size() == 0);
    BOOST_CHECK_NO_THROW(moved[fileSize - 1]);
    BOOST_CHECK_THROW(original[0], std::exception);
}

BOOST_AUTO_TEST_CASE(convertionToBytePointerSanityCheck) {
    SharedFileMemory original(TEST_TEXT_PATH);
    SharedFileMemory moved(std::move(original));
    BOOST_TEST((uint8_t*)original == nullptr);
    BOOST_CHECK_NO_THROW((uint8_t*)moved);
}

BOOST_AUTO_TEST_CASE(bracketOperatorSanityCheck) {
    SharedFileMemory original(TEST_TEXT_PATH);
    SharedFileMemory moved(std::move(original));
    BOOST_CHECK_THROW(original[0], std::exception);
    BOOST_CHECK_NO_THROW(moved[0]);
    BOOST_CHECK_NO_THROW(moved[moved.size() - 1]);
    BOOST_CHECK_THROW(moved[moved.size()], std::exception);
}

} // namespace elevation
