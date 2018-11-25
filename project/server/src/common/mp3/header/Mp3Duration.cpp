#include "Mp3Duration.hpp"
#include <iomanip>

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

std::ostream& operator<< (std::ostream& os, const Mp3Duration& duration) {
    os << std::setfill('0') << std::setw(2);
    os << duration.getMinutes() << ':' << duration.getSeconds();
}