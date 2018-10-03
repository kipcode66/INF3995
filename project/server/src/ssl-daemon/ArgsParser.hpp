#include <inttypes.h>
#include <vector>
#include <string>

namespace elevation {
namespace daemon {

class ArgsParser {
public:
    class Config {
    public:
        constexpr explicit Config(uint16_t listenPort, uint16_t outputPort)
            : m_listenPort(listenPort)
            , m_outputPort(outputPort)
        { }

        uint16_t getListenPort() const { return m_listenPort; }
        uint16_t getOutputPort() const { return m_outputPort; }

    private:
        uint16_t m_listenPort;
        uint16_t m_outputPort;
    };

public:
    explicit ArgsParser();
    Config parseArgs(const std::vector<std::string>& argv);
};

} // namespace daemon
} // namespace elevation
