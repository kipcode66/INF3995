/* #include <pistache/net.h> */
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include "StatsApi.hpp"

using namespace Pistache;


StatsApi::StatsApi(Rest::Description& desc) {
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
            .route(desc.get("/statistiques"))
            .bind(&StatsApi::getSuperviseurStatistiques_, this)
            .hide();
}

std::string StatsApi::generateStatistics_(const Statistics& stats) {
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

void StatsApi::getSuperviseurStatistiques_(const Rest::Request& request,
                                           Http::ResponseWriter response) {
    std::thread([this](const Rest::Request& request, Http::ResponseWriter response) {
        Database* db = Database::instance();
        std::ostringstream logMsg;
        auto t = request.headers().getRaw("X-Auth-Token");
        uint32_t token = std::stoul(t.value());

        bool isAdminConnected;
        try {
            isAdminConnected = db->isAdminConnected(token);
        } catch (sqlite_error& e) { }

        if (token == 0) {
            logMsg << "Could not get the Statistics. Received invalid token.";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Forbidden, "Invalid token");
            return;
        } else if (!isAdminConnected) {
            logMsg << "Could not get the statistics. Admin with token \"" << token << "\" is not connected.";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Forbidden, "Admin not connected");
            return;
        }
        Statistics stats = db->getStatistics();
        std::stringstream resp;
        resp << "{\n\"Statistiques\":[\n" << generateStatistics_(stats) << "\n]\n}\n";
        logMsg << "Statistics for admin \"" << token << "\" were successfuly sent.";
        m_logger.log(logMsg.str());
        response.send(Http::Code::Ok, resp.str());
    }, std::move(request), std::move(response)).detach();
}

