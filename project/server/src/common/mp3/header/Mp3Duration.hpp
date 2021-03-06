#ifndef SRC_HTTP_MP3_DURATION_HPP
#define SRC_HTTP_MP3_DURATION_HPP

#include <inttypes.h>
#include <sstream>
#include <ostream>

class Mp3Duration {
public:
    static constexpr const uint8_t SECONDS_PER_MINUTE = 60;

public:
    explicit Mp3Duration(int durationInSeconds = 0);
    virtual ~Mp3Duration();

    uint32_t getMinutes() const;
    uint32_t getSeconds() const;

    int getDurationInSeconds() const;
    friend std::ostream& operator<< (std::ostream& os, const Mp3Duration& duration);

private:
    int m_durationInSeconds;

};

#endif // !SRC_HTTP_MP3_DURATION_HPP

