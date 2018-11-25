/* #include <pistache/net.h> */
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>
#include <common/database/Database.hpp>
#include <iomanip>
#include "rapidjson/document.h"
#include <pistache/serializer/rapidjson.h>
#include "mp3/header/Mp3Duration.hpp"

#include "StatsApi.hpp"

using namespace Pistache;
using namespace elevation;

StatsApi::StatsApi(Rest::Description& desc, Logger& logger)
    :m_logger(logger)
{
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
            .route(desc.get("/statistiques"))
            .bind(&StatsApi::getSuperviseurStatistiques_, this)
            .hide();
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
        Statistics statistics = db->getStatistics();
        std::stringstream resp;
        resp << "{\n\"Statistiques\":[\n" << statistics << "\n]\n}\n";
        logMsg << "Statistics for admin \"" << token << "\" were successfuly sent.";
        m_logger.log(logMsg.str());
        response.send(Http::Code::Ok, resp.str());
    }, std::move(request), std::move(response)).detach();
}

