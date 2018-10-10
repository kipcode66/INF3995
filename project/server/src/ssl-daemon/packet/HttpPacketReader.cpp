#include "HttpPacketReader.hpp"

namespace elevation {
namespace daemon {

HttpPacketReader::HttpPacketReader(Socket& socket)
    : m_socket(socket)
{

}

HttpPacketReader::~HttpPacketReader() { }

std::string HttpPacketReader::readPacket() {

}

} // namespace daemon
} // namespace elevation
