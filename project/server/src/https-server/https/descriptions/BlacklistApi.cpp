#include <pistache/serializer/rapidjson.h>
#include <common/database/Database.hpp>
#include <common/logger/Logger.hpp>

#include "rapidjson/document.h"
#include "BlacklistApi.hpp"

using namespace Pistache;
using namespace elevation;


BlacklistApi::BlacklistApi(Pistache::Rest::Description& desc, Logger& logger)
    : m_logger(logger)
{
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
        .route(desc.post("/bloquer"))
        .bind(&BlacklistApi::postSuperviseurBloquer_, this)
        .hide();

    superviseurPath
        .route(desc.post("/debloquer"))
        .bind(&BlacklistApi::postSuperviseurDebloquer_, this)
        .hide();

    superviseurPath
        .route(desc.get("/listenoire"))
        .bind(&BlacklistApi::getSuperviseurListenoire_, this)
        .hide();
}

void BlacklistApi::getSuperviseurListenoire_(const Rest::Request& request,
                                             Http::ResponseWriter response) {
    std::thread([this](const Rest::Request& request, Http::ResponseWriter response) {
        Database* db = Database::instance();
        std::ostringstream logMsg;
        uint32_t token;
        try {
            auto t = request.headers().getRaw("X-Auth-Token");
            token = std::stoul(t.value());
        } catch (std::runtime_error& e) {
            response.send(Http::Code::Bad_Request, std::string("Malformed Request - ").append(e.what()));
        }
        if (token == 0) {
            logMsg << "Could not retrieve the blacklist. Received invalid token.";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Forbidden, "Invalid token");
            return;
        }

        if (!db->isAdminConnected(token)) {
            logMsg << "Could not retrieve the blacklist. Admin is not connected with token \"" << token << "\".";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Unauthorized, "Admin not connected");
            return;
        }
        std::vector<User_t> blackList = db->getBlackList();
        std::stringstream resp;
        resp << "{\n\"bloques\":[\n";
        for (auto& user : blackList) {
            resp << generateUser_(user) << (&blackList.back() != &user ? ",\n" : "\n");
        }
        resp << "]\n}\n";
        logMsg << "The blacklist was successfuly sent to admin with id \"" << token << "\"";
        m_logger.log(logMsg.str());
        response.send(Http::Code::Ok, resp.str());
    }, std::move(request), std::move(response)).detach();
}

std::string BlacklistApi::generateUser_(const User_t& user) {
    rapidjson::Document userDoc;
    userDoc.SetObject();
    try {
        userDoc.AddMember(rapidjson::StringRef("ip"), rapidjson::Value(user.ip, strlen(user.ip)), userDoc.GetAllocator());
        userDoc.AddMember(rapidjson::StringRef("MAC"), rapidjson::Value(user.mac, strlen(user.mac)), userDoc.GetAllocator());
        userDoc.AddMember(rapidjson::StringRef("name"), rapidjson::Value(user.name, strlen(user.name)), userDoc.GetAllocator());
    }
    catch (sqlite_error& e) {
        std::stringstream msg;
        msg << "An error occured while generating song a song's json: " << e.what();
        m_logger.err(msg.str());
    }
    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    userDoc.Accept(writer);

    return buf.GetString();
}

void BlacklistApi::postSuperviseurBloquer_(const Rest::Request& request,
                                            Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "user blocked");
}

void BlacklistApi::postSuperviseurDebloquer_(const Rest::Request& request,
                                              Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "user unblocked");
}

