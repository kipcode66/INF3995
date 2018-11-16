#include "Mp3Event.hpp"

#include <sstream>

namespace elevation {

Mp3Event::Mp3Event() { }
Mp3Event::~Mp3Event() { }

std::string Mp3Event::serialize() const {
    std::ostringstream messageStream;
    messageStream << std::bitset<sizeof(EVENT_SIGNATURE)>(EVENT_SIGNATURE);
    return messageStream.str();
}

void Mp3Event::padToDataSize(std::ostream& dataStream) {
    std::streampos initialWritePos = dataStream.tellp();
    dataStream.seekp(0, std::ios::end);
    std::size_t currentDataSize = dataStream.tellp();
    dataStream.seekp(initialWritePos);

    while (currentDataSize < DATA_SIZE) {
        dataStream << PADDING;
    }
}

} // namespace elevation
