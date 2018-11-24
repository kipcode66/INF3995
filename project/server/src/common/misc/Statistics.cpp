#include "Statistics.hpp"

using namespace elevation;

Statistics::Statistics(int nSongs, int nUsers, int nDeletedSongs, int avgDuration) :
    m_SongCount(nSongs),
    m_UserCount(nUsers),
    m_DeletedSongsCount(nDeletedSongs),
    m_averageDurationInSec(avgDuration)
{}

int Statistics::getSongCount() const {
    return m_SongCount;
}

int Statistics::getUserCount() const {
    return m_UserCount;
}

int Statistics::getDeletedSongsCount() const {
    return m_DeletedSongsCount;
}

int Statistics::getAverageDuration() const {
    return m_averageDurationInSec;
}
