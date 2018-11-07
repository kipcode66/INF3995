#ifndef SRC_HTTP_MP3_DURATION_HPP
#define SRC_HTTP_MP3_DURATION_HPP

#include <inttypes.h>

class Mp3Duration {
public:
    explicit Mp3Duration(int durationInSeconds = 0);
    virtual ~Mp3Duration();

    uint32_t getMinutes();
    uint32_t getSeconds();

private:
    uint32_t m_minutes;
    uint32_t m_seconds;

};

#endif // !SRC_HTTP_MP3_DURATION_HPP

