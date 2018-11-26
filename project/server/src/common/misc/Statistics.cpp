#include "Statistics.hpp"
#include "rapidjson/document.h"
#include <pistache/serializer/rapidjson.h>
#include <iomanip>
#include <database/sqlite_error.hpp>
#include <common/rest/rest_utils.hpp>

using namespace elevation;

Statistics::Statistics(int nSongs, int nUsers, int nDeletedSongs, int avgDuration) :
    m_songCount(nSongs),
    m_userCount(nUsers),
    m_deletedSongsCount(nDeletedSongs),
    m_averageDurationInSec(avgDuration)
{}

int Statistics::getSongCount() const {
    return m_songCount;
}

int Statistics::getUserCount() const {
    return m_userCount;
}

int Statistics::getDeletedSongsCount() const {
    return m_deletedSongsCount;
}

int Statistics::getAverageDuration() const {
    return m_averageDurationInSec;
}

std::ostream& elevation::operator<<(std::ostream& os, const Statistics& stats) {
    return os << rest_utils::generateStatistics_(stats);
}
