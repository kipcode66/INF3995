#ifndef ARGS_PARSER_HPP
#define ARGS_PARSER_HPP

#include <vector>
#include <string>

namespace elevation {

class ArgsParser {
public:
    explicit ArgsParser(const std::vector<std::string>& args);

    std::string getCachePath() const { return m_cachePath; }

protected:
    void usage_();

protected:
    std::string m_cachePath;
};

} // namespace elevation

#endif // !ARGS_PARSER_HPP
