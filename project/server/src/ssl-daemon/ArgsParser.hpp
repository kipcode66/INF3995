#ifndef ARGS_PARSER_HPP
#define ARGS_PARSER_HPP

#include <inttypes.h>
#include <vector>
#include <string>

namespace elevation {
namespace daemon {

class ArgsParser {
public:
    static constexpr uint16_t DEFAULT_LISTEN_PORT = 443;
    static constexpr uint16_t DEFAULT_OUTPUT_PORT = 7777;

public:
    class Config {
    public:
        constexpr explicit Config(uint16_t listenPort, uint16_t outputPort)
            : m_listenPort(listenPort)
            , m_outputPort(outputPort)
        { }

        uint16_t getListenPort() const { return m_listenPort; }
        uint16_t getOutputPort() const { return m_outputPort; }

        bool operator==(const Config& other) const { // For unit tests
            return m_listenPort == other.m_listenPort &&
                   m_outputPort == other.m_outputPort;
        }

    private:
        uint16_t m_listenPort;
        uint16_t m_outputPort;
    };

public:
    explicit ArgsParser();
    Config parseArgs(const std::vector<std::string>& argv) const;
    void usage() const;
};

} // namespace daemon
} // namespace elevation

#endif // !ARGS_PARSER_HPP
