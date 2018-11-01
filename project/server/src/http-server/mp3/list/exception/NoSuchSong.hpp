#ifndef MP3_LIST_EXCEPTION_NOSUCHSONG_HPP
#define MP3_LIST_EXCEPTION_NOSUCHSONG_HPP

#include <stdexcept>
#include <string>

namespace elevation {

class NoSuchSong : public std::exception {
public:
    static const std::string MESSAGE_PREFIX;

public:
    explicit NoSuchSong(const std::string& m_message);
    virtual const char* what() const noexcept override;

protected:
    std::string m_message;
};

}

#endif // !MP3_LIST_EXCEPTION_NOSUCHSONG_HPP
