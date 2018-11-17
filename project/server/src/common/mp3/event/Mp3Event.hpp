#ifndef MP3_EVENT_MP3EVENT_HPP
#define MP3_EVENT_MP3EVENT_HPP

#include <inttypes.h>
#include <string>
#include <sstream>
#include <bitset>

namespace elevation {

class Mp3Event {
public:

    enum class EventType : uint16_t {
        NONE = 0,
        VOLUME_CHANGE,
        NUM_EVENTS
    };

public:
    static constexpr uint32_t EVENT_SIGNATURE = 0x8d8315d4; ///< Bytes which indicate that the transmitted data is an event and not another kind of packet.
    static constexpr char PADDING = '\0';
    static constexpr std::size_t DATA_SIZE = 16; ///< Size (in B) of serialized data of subclasses.
    static constexpr std::size_t PAYLOAD_SIZE = DATA_SIZE - sizeof(EVENT_SIGNATURE) - sizeof(EventType); ///< Size (in B) of serialized payload.

public:
    explicit Mp3Event(EventType eventType);
    virtual ~Mp3Event() = 0;

    virtual std::string serialize() const = 0;

    /**
     * @brief Serializes a single integer/binary element as binary into a string.
     */
    template <class T>
    static std::string serializeElement(T element) {
        std::ostringstream elementStream;
        elementStream.write(reinterpret_cast<char*>(&element), sizeof(T));
        std::string serializedData = elementStream.str();
        return serializedData;
    }

    /**
     * @brief Adds null bytes to the given stream so that it has exactly #DATA_SIZE bytes.
     */
    static void padToDataSize(std::ostream& dataStream);

protected:
    EventType m_eventType;
};

} // namespace elevation

#endif // !MP3_EVENT_MP3EVENT_HPP
