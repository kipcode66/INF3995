#include <pistache/serializer/rapidjson.h>
#include <common/database/Database.hpp>
#include <common/logger/Logger.hpp>
#include <common/database/templates/exception/NoSuchUserException.hpp>

#include "rapidjson/document.h"
#include "BlacklistApi.hpp"

using namespace Pistache;

namespace elevation {

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
        try {
            if(!checkIfAdmin_(request)) {
                response.send(Http::Code::Unauthorized, "Admin not connected");
                return;
            }
        } catch (const std::exception& e) {
            response.send(Http::Code::Bad_Request, std::string("Malformed request - ").append(e.what()));
            return;
        }
        std::vector<User_t> blackList = db->getBlackList();
        std::stringstream resp;
        resp << "{\"bloques\":[";
        for (auto& user : blackList) {
            resp << generateUser_(user) << (&blackList.back() != &user ? "," : "");
        }
        resp << "]}";
        logMsg << "The blacklist was successfuly sent to admin";
        m_logger.log(logMsg.str());
        response.send(Http::Code::Ok, resp.str());
    }, std::move(request), std::move(response)).detach();
}

std::string BlacklistApi::generateUser_(const User_t& user) {
    rapidjson::Document userDoc;
    userDoc.SetObject();
    try {
        userDoc.AddMember(rapidjson::StringRef("ip"), rapidjson::Value(user.ip, strlen(user.ip)), userDoc.GetAllocator());
        userDoc.AddMember(rapidjson::StringRef("mac"), rapidjson::Value(user.mac, strlen(user.mac)), userDoc.GetAllocator());
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

bool BlacklistApi::checkIfAdmin_(const Rest::Request& request) {
    std::ostringstream logMsg;

    uint32_t token = std::stoul(request.headers().getRaw("X-Auth-Token").value());
    Database* db = Database::instance();
    if (token != 0 && !db->isAdminConnected(token)) {
        logMsg << "Could not retrieve the blacklist. Admin is not connected with token \"" << token << "\".";
        m_logger.err(logMsg.str());
        return false;
    } else {
        return true;
    }
}

void BlacklistApi::postSuperviseurBloquer_(const Rest::Request& request,
                                           Http::ResponseWriter response) {
    std::thread([this](const Rest::Request& request, Http::ResponseWriter response) {
        rapidjson::Document jsonDocument;
        jsonDocument.Parse(request.body().c_str());

        bool isValid = (jsonDocument.IsObject() &&
                        jsonDocument.HasMember("ip") &&
                        jsonDocument.HasMember("mac") &&
                        jsonDocument.HasMember("nom"));
        if (!isValid) {
            response.send(Http::Code::Bad_Request, "Malformed request");
            return;
        }
        try {
            if(!checkIfAdmin_(request)) {
                response.send(Http::Code::Unauthorized, "Admin not connected");
                return;
            }
        } catch (const std::exception& e) {
            response.send(Http::Code::Bad_Request, std::string("Malformed request - ").append(e.what()));
            return;
        }
        Database* db = Database::instance();
        std::string mac(jsonDocument["mac"].GetString());
        try {
            if (db->getBlacklistByMAC(mac)) {
                response.send(Http::Code::Ok, "No change made - user already blocked");
            } else {
                db->blacklistMAC(mac);
                response.send(Http::Code::Ok, "User blocked");
            }
        } catch (const NoSuchUserException& e) {
            response.send(Http::Code::Bad_Request, e.what());
        } catch (const std::exception& e) {
            response.send(Http::Code::Internal_Server_Error, std::string("unknown error - ").append(e.what()));
        }
    }, std::move(request), std::move(response)).detach();
}

void BlacklistApi::postSuperviseurDebloquer_(const Rest::Request& request,
                                             Http::ResponseWriter response) {
    std::thread([this](const Rest::Request& request, Http::ResponseWriter response) {
        Database* db = Database::instance();
        rapidjson::Document jsonDocument;
        jsonDocument.Parse(request.body().c_str());
        bool isValid = (jsonDocument.IsObject() &&
                        jsonDocument.HasMember("mac"));
        if (!isValid) {
            response.send(Http::Code::Bad_Request, "Malformed request");
            return;
        }
        std::string mac(jsonDocument["mac"].GetString());
        try {
            if (db->getBlacklistByMAC(mac)) {
                db->whitelistMAC(mac);
                response.send(Http::Code::Ok, "user unblocked");
            } else {
                response.send(Http::Code::Ok, "No change made - user not blocked");
            }
        } catch (std::exception& e) {
            response.send(Http::Code::Internal_Server_Error, std::string("unknown error - ").append(e.what()));
        }
        response.send(Http::Code::Ok, "user unblocked");
    }, std::move(request), std::move(response)).detach();
}

} // namespace elevation
