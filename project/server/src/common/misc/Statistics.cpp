#include "Statistics.hpp"
#include "rapidjson/document.h"
#include <pistache/serializer/rapidjson.h>
#include <iomanip>
#include <database/sqlite_error.hpp>

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

std::string Statistics::generateStatistics() const {
    rapidjson::Document statsDoc;
    statsDoc.SetObject();
    try {
        Mp3Duration songDuration(m_averageDurationInSec);
        std::stringstream duration;
        duration << songDuration;
        statsDoc.AddMember(rapidjson::StringRef("chansons"), rapidjson::Value(m_songCount), statsDoc.GetAllocator());
        statsDoc.AddMember(rapidjson::StringRef("utilisateurs"), rapidjson::Value(m_userCount), statsDoc.GetAllocator());
        statsDoc.AddMember(rapidjson::StringRef("elimines"), rapidjson::Value(m_deletedSongsCount), statsDoc.GetAllocator());
        statsDoc.AddMember(rapidjson::StringRef("temps"), rapidjson::Value(duration.str().c_str(), duration.str().length()), statsDoc.GetAllocator());
    }
    catch (sqlite_error& e) { }
    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    statsDoc.Accept(writer);

    return buf.GetString();
}

std::ostream& elevation::operator<<(std::ostream& os, const Statistics& stats) {
    return os << stats.generateStatistics();
}
