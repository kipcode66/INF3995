#include <common/logger/Logger.hpp>
#include <pistache/serializer/rapidjson.h>
#include <future>

#include "SecureRestApi.hpp"
#include "rapidjson/document.h"
#include "misc/id_utils.hpp"
#include "database/Database.hpp"


#include <common/logger/Logger.hpp>

using namespace elevation;

SecureRestApi::SecureRestApi(Address addr, Logger& logger)
: RestApi(addr, logger)
, m_logger(logger)
{ }

void SecureRestApi::init() {
    auto opts = Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    createDescription_();
    createSecureDescription_();
}

void SecureRestApi::createSecureDescription_() {
    auto superviseurPath = m_desc.path("/superviseur");

    superviseurPath
            .route(m_desc.get("/file"))
            .bind(&SecureRestApi::getSuperviseurFile_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/changement_mdp"))
            .bind(&SecureRestApi::postChangePassword_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/login"))
            .bind(&SecureRestApi::superviseurLogin_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/logout"))
            .bind(&SecureRestApi::superviseurLogout_, this)
            .hide();
}

void SecureRestApi::getSuperviseurFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurFile called");
    std::ostringstream logMsg;
    logMsg << "getSuperviseurFile called ";
    m_logger.log(logMsg.str());
}

void SecureRestApi::superviseurLogin_(const Rest::Request& request, Http::ResponseWriter response) {
    auto body = request.body();
    rapidjson::Document request_json;
    request_json.Parse(body.c_str());
    std::ostringstream logMsg;
    if ((!request_json.IsObject()
            || (!request_json.HasMember("usager")
                || !request_json.HasMember("mot_de_passe")
                || request_json["usager"] == '\n'
                || request_json["mot_de_passe"] == '\n'))) {

        logMsg << "Could not login the admin. The request is malformed.";
        m_logger.err(logMsg.str());
        response.send(Http::Code::Bad_Request, "Malformed request");
        return;
    }
    auto t = request.headers().getRaw("X-Auth-Token");
    if (t.value().empty()) {
        std::cout << "Header \"X-Auth-Token\" missing" << std::endl;
        logMsg << "Could not login the admin. Header \"X-Auth-Token\" missing.";
        m_logger.err(logMsg.str());
        response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" missing");
        return;
    }
    std::string login(request_json["usager"].GetString());
    if (login != ADMIN_USERNAME) {
        logMsg << "Could not login the admin. Received wrong login";
        m_logger.err(logMsg.str());
        response.send(Http::Code::Bad_Request, "Wrong login");
        return;
    }
    uint32_t adminId = std::stoul(t.value());
    if (adminId == 0) {
        logMsg << "Received an invalid token to login admin. Login Aborted.";
        m_logger.err(logMsg.str());
        response.send(Http::Code::Forbidden, "Invalid token");
        return;
    }
    std::async([&]() {
        Database* db = Database::instance();
        if (db->isAdminConnected(adminId)) {
            logMsg << "Could not Login Admin. Admin with token \"" << adminId << "\" is already connected.";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Bad_Request, "Admin already connected with this token");
            return;
        }
        auto saltAndPasswordHash = db->getSaltAndHashedPasswordByLogin(login.c_str());
        std::string salt = std::get<0>(saltAndPasswordHash);
        std::string hash = std::get<1>(saltAndPasswordHash);
        std::string password(request_json["mot_de_passe"].GetString());
        std::string passwordHash = elevation::id_utils::generateMd5Hash(password, salt);
        if (passwordHash == hash) {
            db->connectAdmin(login.c_str(), adminId);
            logMsg << "Successfuly logged in admin with token \"" << adminId << "\".";
            m_logger.log(logMsg.str());
            response.send(Http::Code::Ok, "Connexion successful");
            return;
        } else {
            logMsg << "Could not Login admin with token \"" << adminId << "\". Received wrong password";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Forbidden, "Wrong password");
            return;
        }
    });
}

void SecureRestApi::superviseurLogout_(const Rest::Request& request, Http::ResponseWriter response) {
    auto t = request.headers().getRaw("X-Auth-Token");
    std::ostringstream logMsg;
    if (t.value().empty()) {
        logMsg << "Could not login the admin. Header \"X-Auth-Token\" missing.";
        m_logger.err(logMsg.str());
        response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" missing");
        return;
    }
    uint32_t adminId = std::stoul(t.value());
    if (adminId == 0) {
        logMsg << "Received an invalid token to logout admin. Logout Aborted.";
        m_logger.err(logMsg.str());
        response.send(Http::Code::Forbidden, "Invalid token");
        return;
    }
    std::async([&]() {
        Database* db = Database::instance();
        if (!db->isAdminConnected(adminId)) {
            logMsg << "Could not logout admin with token \"" << adminId << "\". Admin was not already logged in.";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Unauthorized, "Admin not connected");
            return;
        } else {
            logMsg << "Admin with token \"" << adminId << "\" was successfuly logged out.";
            m_logger.log(logMsg.str());
            db->disconnectAdmin(adminId);
            response.send(Http::Code::Ok, "Logout successful");
            return;
        }
    });
}

void SecureRestApi::postChangePassword_(const Rest::Request& request, Http::ResponseWriter response) {
    try {
        auto body = request.body();
        rapidjson::Document request_json;
        request_json.Parse(body.c_str());
        if (!request_json.HasMember("ancien") || !request_json.HasMember("nouveau")) {
            response.send(Http::Code::Bad_Request, "Malformed request");
            return;
        }
        std::string ancien(request_json["ancien"].GetString());
        std::string nouveau(request_json["nouveau"].GetString());
        std::cout << "changing " << ancien << " to " << nouveau  << std::endl;
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
    response.send(Http::Code::Ok, "chgm_mdp");
}
