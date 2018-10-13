#include "HttpPacketReader.hpp"

#include <sstream>

namespace elevation {
namespace daemon {

const std::regex HttpPacketReader::s_HTTP_HEADER_END_REGEX("^\r?\n$");
const std::regex HttpPacketReader::s_HTTP_CONTENT_SIZE_REGEX("^Content-Length:\\s*(\\d+)", std::regex_constants::icase);
const std::size_t HttpPacketReader::s_HTTP_CONTENT_SIZE_RESULT_GROUP_ID = 1;

HttpPacketReader::HttpPacketReader(Socket& socket)
    : m_socket(socket)
{

}

HttpPacketReader::~HttpPacketReader() { }

std::string HttpPacketReader::readPacket() {
    std::ostringstream dataStream;

    std::size_t httpBodySize = 0;
    std::string line = m_socket.readLine();
    while (!std::regex_search(line, s_HTTP_HEADER_END_REGEX)) {
        std::smatch matchResult;
        if (std::regex_search(line, matchResult, s_HTTP_CONTENT_SIZE_REGEX)) {
            httpBodySize = std::stoi(matchResult[s_HTTP_CONTENT_SIZE_RESULT_GROUP_ID]);
        }

        dataStream << std::move(line);
        line = m_socket.readLine();
    }
    dataStream << std::move(line);
    std::string data = m_socket.read(httpBodySize);
    dataStream << std::move(data);

    return dataStream.str();
}

} // namespace daemon
} // namespace elevation
