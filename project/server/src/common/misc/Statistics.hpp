#include "mp3/header/Mp3Duration.hpp"

#ifndef MISC_STATISTICS_HPP
#define MISC_STATISTICS_HPP

namespace elevation {
class Statistics {

public:
    Statistics(int, int, int, int);

    int getSongCount() const;
    int getUserCount() const;
    int getDeletedSongsCount() const;
    int getAverageDuration() const;
    std::string generateStatistics() const;

    friend std::ostream& operator<<(std::ostream& os, const Statistics& stats);

public:
    int m_songCount;
    int m_userCount;
    int m_deletedSongsCount;
    int m_averageDurationInSec;

};
}// namespace elevation

#endif
