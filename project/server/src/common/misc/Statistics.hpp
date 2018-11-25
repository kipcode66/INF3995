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

public:
    int m_songCount;
    int m_userCount;
    int m_deletedSongsCount;
    int m_averageDurationInSec;

};
}// namespace elevation

#endif