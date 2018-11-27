#ifndef PACKET_HTTPPACKETREADER_HPP
#define PACKET_HTTPPACKETREADER_HPP

#include <string>
#include <regex>

#include <common/os/Socket.hpp>

namespace elevation {
namespace daemon {

class HttpPacketReader {

public:
    static const std::regex s_HTTP_HEADER_END_REGEX;
    static const std::regex s_HTTP_CONTENT_SIZE_REGEX;
    static const std::size_t s_HTTP_CONTENT_SIZE_RESULT_GROUP_ID;

public:
    explicit HttpPacketReader(std::shared_ptr<Socket> socket);
    virtual ~HttpPacketReader();

    std::string readPacket();

protected:
    std::shared_ptr<Socket> m_socket;
};

} // namespace daemon
} // namespace elevation

#endif // !PACKET_HTTPPACKETREADER_HPP
