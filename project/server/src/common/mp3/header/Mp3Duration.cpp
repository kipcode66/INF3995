#include "Mp3Duration.hpp"

Mp3Duration::Mp3Duration(int durationInSeconds)
{
    const uint8_t SECONDS_PER_MINUTE = 60;
    m_minutes = durationInSeconds / SECONDS_PER_MINUTE;
    m_seconds = durationInSeconds % SECONDS_PER_MINUTE;
}

Mp3Duration::~Mp3Duration()
{ }

uint32_t Mp3Duration::getMinutes() {
    return m_minutes;
}

uint32_t Mp3Duration::getSeconds() {
    return m_seconds;
}