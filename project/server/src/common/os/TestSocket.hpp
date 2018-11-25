#ifndef OS_TESTSOCKET_HPP
#define OS_TESTSOCKET_HPP

#include <vector>

#include "os/Socket.hpp"

namespace elevation {

/**
 * @brief A class useful for unit tests.
 * @details This class throws the SocketClosedException
 * whenever a read attempt fails because there is not
 * enough data in the write buffer.
 */
class TestSocket : public Socket {
public:
    explicit TestSocket();
    virtual ~TestSocket() = default;

    virtual void writeRaw(const std::string& str) override;
    virtual void write(const std::string& str) override;
    virtual std::string readLine(char end = '\n') override;
    virtual std::string read(std::size_t dataLength) override;

    std::string getWrittenData() { return m_writtenData; }

protected:
    void assertNotClosed_() const;

    std::string consumeDataFromBuffer_(std::string::const_iterator beg, std::string::const_iterator end);

protected:
    std::string m_writtenData;
    bool m_isClosed;
};

} // namespace elevation

#endif // !OS_TESTSOCKET_HPP
