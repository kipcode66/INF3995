#ifndef ARGS_PARSER_HPP
#define ARGS_PARSER_HPP

#include <string>

namespace elevation {

class ArgsParser {
public:
    explicit ArgsParser(int argc, char** argv);

    std::string getCachePath() const { return m_cachePath; }

protected:
    void usage();

protected:
    std::string m_cachePath;
};

} // namespace elevation

#endif // !ARGS_PARSER_HPP
