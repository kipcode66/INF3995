#include "StatsApi.hpp"

#include <iomanip>

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>
#include <pistache/serializer/rapidjson.h>

#include <rapidjson/document.h>

#include <common/database/Database.hpp>

#include "mp3/header/Mp3Duration.hpp"

using namespace Pistache;

namespace elevation {

StatsApi::StatsApi(Rest::Description& desc, Logger& logger)
    : m_logger(logger)
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
        try {
            resp << statistics;
        } catch (sqlite_error& e) {
            std::stringstream msg;
            msg << "An error occured while generating a stats json: " << e.what();
            m_logger.err(msg.str());
        }
        logMsg << "Statistics for admin \"" << token << "\" were successfuly sent.";
        m_logger.log(logMsg.str());
        response.send(Http::Code::Ok, resp.str());
    }, std::move(request), std::move(response)).detach();
}

} // namespace elevation
