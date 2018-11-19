#include "TestSocket.hpp"
#include "exception/SocketClosedException.hpp"

#include <algorithm>

namespace elevation {

TestSocket::TestSocket()
    : Socket(0, s_NO_FD)
    , m_isClosed(false)
{ }

void TestSocket::writeRaw(const std::string& str) {
    assertNotClosed_();
    m_writtenData += str;
}

void TestSocket::write(const std::string& str) {
    assertNotClosed_();
    m_writtenData += (str + '\0');
}

std::string TestSocket::readLine(char end) {
    assertNotClosed_();

    auto endIt = std::find(m_writtenData.begin(), m_writtenData.end(), end);
    if (endIt != m_writtenData.end()) {
        ++endIt;
    }
    std::string nextLine = consumeDataFromBuffer_(m_writtenData.begin(), endIt);

    if (nextLine.empty() || nextLine.back() != end) {
        m_isClosed = true;
        throw SocketClosedException();
    }
    return nextLine;
}

std::string TestSocket::read(std::size_t dataLength) {
    assertNotClosed_();

    if (m_writtenData.size() < dataLength) {
        m_writtenData = "";
        m_isClosed = true;
        throw SocketClosedException();
    }
    std::string readData =
        consumeDataFromBuffer_(
            m_writtenData.begin(),
            std::next(m_writtenData.begin(), dataLength)
        );
    return readData;
}

std::string TestSocket::consumeDataFromBuffer_(std::string::const_iterator beg, std::string::const_iterator end) {
    std::string consumedData;
    std::copy(beg, end, std::back_inserter(consumedData));
    m_writtenData.erase(beg, end);
    return consumedData;
}

void TestSocket::assertNotClosed_() const {
    if (m_isClosed) {
        throw SocketClosedException();
    }
}

} // namespace elevation
