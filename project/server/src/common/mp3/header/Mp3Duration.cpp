#include "Mp3Duration.hpp"

Mp3Duration::Mp3Duration(int durationInSeconds)
: m_durationInSeconds(durationInSeconds)
{ }

Mp3Duration::~Mp3Duration()
{ }

uint32_t Mp3Duration::getMinutes() const {
    return m_durationInSeconds / SECONDS_PER_MINUTE;
}

uint32_t Mp3Duration::getSeconds() const {
    return m_durationInSeconds % SECONDS_PER_MINUTE;
}

int Mp3Duration::getDurationInSeconds() const {
    return m_durationInSeconds;
}