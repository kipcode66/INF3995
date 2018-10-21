#ifndef MP3_PLAYER_MAD_MADSTREAM_HPP
#define MP3_PLAYER_MAD_MADSTREAM_HPP

#include <vector>
#include <inttypes.h>
#include <memory>

namespace elevation {

class MadStream {
public:
    virtual ~MadStream();

    static void createInstance(std::vector<uint8_t> buffer);
    static MadStream& getInstance();

private:
    explicit MadStream(std::vector<uint8_t> buffer);

protected:
    static std::unique_ptr<MadStream> s_instance;

    std::vector<uint8_t> m_buffer;
};

}

#endif // !MP3_PLAYER_MAD_MADSTREAM_HPP
