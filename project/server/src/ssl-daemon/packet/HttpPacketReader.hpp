#ifndef PACKET_HTTPPACKETREADER_HPP
#define PACKET_HTTPPACKETREADER_HPP

#include <string>

#include "communication/Socket.hpp"

namespace elevation {
namespace daemon {

class HttpPacketReader {

public:
    explicit HttpPacketReader(Socket& socket);
    virtual ~HttpPacketReader();

    std::string readPacket();

protected:
    Socket& m_socket;
};

} // namespace daemon
} // namespace elevation

#endif // !PACKET_HTTPPACKETREADER_HPP
