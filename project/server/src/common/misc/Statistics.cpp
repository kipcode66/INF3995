#include "Statistics.hpp"
#include "rapidjson/document.h"

using namespace elevation;

Statistics::Statistics(int nSongs, int nUsers, int nDeletedSongs, int avgDuration) :
    m_numberOfSongs(nSongs),
    m_numberOfUsers(nUsers),
    m_numberOfdeletedSongs(nDeletedSongs),
    m_averageDurationInSec(avgDuration)
{}

int Statistics::getSongCount() const {
    return m_SongCount;
}

int Statistics::getUserCount() const; {
    return m_UserCount;
}

int Statistics::getDeletedSongsCount() const {
    return m_DeletedSongsCount;
}

int Statistics::getAverageDuration() const {
    return m_averageDurationInSec;
}

std::string Statistics::generateStatisticsJson() {
    rapidjson::Document statsDoc;
    statsDoc.SetObject();
    try {
        statsDoc.AddMember(rapidjson::StringRef("chansons"), rapidjson::Value(stats.getSongCount()), statsDoc.GetAllocator());
        statsDoc.AddMember(rapidjson::StringRef("utilisateurs"), rapidjson::Value(stats.getUserCount()), statsDoc.GetAllocator());
        statsDoc.AddMember(rapidjson::StringRef("elimines"), rapidjson::Value(stats.getDeletedSongsCount()), statsDoc.GetAllocator());
        statsDoc.AddMember(rapidjson::StringRef("temps"), rapidjson::Value(stats.getAverageDuration()), statsDoc.GetAllocator());
    }
    catch (sqlite_error& e) {
        std::stringstream msg;
        msg << "An error occured while generating a statistics's json: " << e.what();
        m_logger.err(msg.str());
    }
    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    statsDoc.Accept(writer);

    return buf.GetString();
}