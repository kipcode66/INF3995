#ifndef MP3_EVENT_MP3EVENT_HPP
#define MP3_EVENT_MP3EVENT_HPP

#include <inttypes.h>
#include <string>
#include <sstream>
#include <bitset>

namespace elevation {

class Mp3Event {
public:
    static constexpr uint32_t EVENT_SIGNATURE = 0x8d8315d4; ///< Bytes which indicate that the transmitted data is an event and not another kind of packet.
    static constexpr std::size_t DATA_SIZE = 16; ///< Size (in B) of serialized data of subclasses.
    static constexpr char PADDING = '\0';

public:
    explicit Mp3Event();
    virtual ~Mp3Event() = 0;

    virtual std::string serialize() const = 0;

    /**
     * @brief Serializes a single integer as binary into a string.
     */
    template <class T>
    static std::string serializeInteger(T integer) {
        std::ostringstream integerStream;
        integerStream << std::bitset<sizeof(T)>(integer);
        return integerStream.str();
    }

    /**
     * @brief Adds null bytes to the given stream so that it has exactly #DATA_SIZE bytes.
     */
    static void padToDataSize(std::ostream& dataStream);
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENT_HPP
