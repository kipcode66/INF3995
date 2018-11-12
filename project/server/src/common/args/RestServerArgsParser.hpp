#ifndef RESTSERVERARGSPARSER_HPP
#define RESTSERVERARGSPARSER_HPP

#include <inttypes.h>
#include <vector>
#include <string>

namespace elevation {

/**
 * @brief Parser of main() arguments for the http-server as well as the https-server.
 */
class RestServerArgsParser {
public:
    explicit RestServerArgsParser(const std::vector<std::string>& args);

    uint32_t    getPort()      const { return m_port;      }
    std::string getCachePath() const { return m_cachePath; }

protected:
    void usage_() const;

protected:
    uint32_t m_port;
    std::string m_cachePath;
};

} // namespace elevation

#endif // !RESTSERVERARGSPARSER_HPP
