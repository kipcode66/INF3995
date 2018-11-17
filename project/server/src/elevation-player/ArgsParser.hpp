#ifndef ARGS_PARSER_HPP
#define ARGS_PARSER_HPP

#include <vector>
#include <string>

namespace elevation {

class ArgsParser {
public:
    explicit ArgsParser(const std::vector<std::string>& args);

    std::string getCachePath() const { return m_cachePath; }
    uint16_t    getPort()      const { return m_port; }

protected:
    void usage_();

protected:
    std::string m_cachePath;
    uint16_t m_port;
};

} // namespace elevation

#endif // !ARGS_PARSER_HPP
