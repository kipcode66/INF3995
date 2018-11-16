#ifndef MP3_EVENT_MP3EVENT_HPP
#define MP3_EVENT_MP3EVENT_HPP

#include <inttypes.h>
#include <string>

namespace elevation {

class Mp3Event {
public:
    static constexpr uint32_t EVENT_SIGNATURE = 0x8d8315d4; ///< Bytes which indicate that the transmitted data is an event and not another kind of packet.
    static constexpr std::size_t DATA_SIZE = 16; ///< Size (in B) of serialized data of subclasses.

public:
    explicit Mp3Event();
    virtual ~Mp3Event() = 0;

    virtual std::string serialize() const = 0;
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENT_HPP
