#include "Statistics.hpp"
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
    std::string generateStatisticsJson();

public:
    int m_SongCount;
    int m_UserCount;
    int m_DeletedSongsCount;
    int m_averageDurationInSec;

};
}// namespace elevation

#endif