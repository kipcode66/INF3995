#include <common/logger/Logger.hpp>
#include <pistache/serializer/rapidjson.h>
#include <future>

#include "SecureRestApi.hpp"
#include "rapidjson/document.h"
#include "misc/id_utils.hpp"
#include "database/Database.hpp"

#include "rapidjson/document.h"

#include <common/database/Database.hpp>
#include <common/logger/Logger.hpp>

using namespace elevation;

SecureRestApi::SecureRestApi(Address addr, Logger& logger, FileCache& cache)
: RestApi(addr, logger, cache)
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

    superviseurPath
            .route(m_desc.post("/changement_mdp"))
            .bind(&SecureRestApi::postChangePassword_, this)
            .hide();
}

void SecureRestApi::getSuperviseurFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurFile called");
    std::ostringstream logMsg;
    logMsg << "getSuperviseurFile called ";
    m_logger.log(logMsg.str());
}


void SecureRestApi::superviseurLogin_(const Rest::Request& request, Http::ResponseWriter response) {
    std::ostringstream logMsg;
    try {
        Admin admin(request);
        if (admin.usager != ADMIN_USERNAME) {
            logMsg << "Could not login the admin. Received wrong login";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Bad_Request, "Wrong login");
            return;
        }
        std::async([&]() {
            Database* db = Database::instance();
            if (db->isAdminConnected(admin.id)) {
                logMsg << "Could not Login Admin. Admin with token \"" << admin.id << "\" is already connected.";
                m_logger.err(logMsg.str());
                response.send(Http::Code::Bad_Request, "Admin already connected with this token");
                return;
            }
            auto saltAndPasswordHash = db->getSaltAndHashedPasswordByLogin(admin.usager.c_str());
            std::string salt = std::get<0>(saltAndPasswordHash);
            std::string hash = std::get<1>(saltAndPasswordHash);
            std::string passwordHash = elevation::id_utils::generateMd5Hash(admin.mot_de_passe, salt);
            if (passwordHash == hash) {
                db->connectAdmin(admin.usager.c_str(), admin.id);
                logMsg << "Successfuly logged in admin with token \"" << admin.id << "\".";
                m_logger.log(logMsg.str());
                response.send(Http::Code::Ok, "Connexion successful");
                return;
            } else {
                logMsg << "Could not Login admin with token \"" << admin.id << "\". Received wrong password";
                m_logger.err(logMsg.str());
                response.send(Http::Code::Forbidden, "Wrong password");
                return;
            }
        });
    } catch (std::exception& e) {
        response.send(Http::Code::Bad_Request, std::string("Malformed request - ").append(e.what()));
        return;
    }
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
        rapidjson::Document jsonDocument;
        jsonDocument.Parse(request.body().c_str());
        bool isValid = (jsonDocument.HasMember("ancien") &&
                        jsonDocument.HasMember("nouveau"));
        if (!isValid) {
            response.send(Http::Code::Bad_Request, "Malformed request");
            return;
        }
        std::string ancien(jsonDocument["ancien"].GetString());
        std::string nouveau(jsonDocument["nouveau"].GetString());
        if (nouveau == ancien) {
            response.send(Http::Code::Ok, "No action performed");
            return;
        }

        Database* db = Database::instance();
        auto saltAndPasswordHash = db->getSaltAndHashedPasswordByLogin(Database::ADMIN_NAME);
        std::string salt = std::get<0>(saltAndPasswordHash);
        std::string passHashFromDB = std::get<1>(saltAndPasswordHash);
        if (elevation::id_utils::generateMd5Hash(ancien, salt) == passHashFromDB) {
            db->createAdmin(nouveau);
            response.send(Http::Code::Ok, "password changed");
        } else {
            response.send(Http::Code::Bad_Request, "old password incorrect");
        }
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
    return;
}

SecureRestApi::Admin::Admin(const Rest::Request& req) {
    rapidjson::Document jsonDocument;
    jsonDocument.Parse(req.body().c_str());
    bool fieldsValid = (jsonDocument.IsObject()
                     && jsonDocument.HasMember("usager")
                     && jsonDocument.HasMember("mot_de_passe")
                     && jsonDocument["usager"]       != '\0'
                     && jsonDocument["mot_de_passe"] != '\0');
    if (!fieldsValid) {
        throw std::runtime_error("missing fields");
    }
    auto token = req.headers().getRaw("X-Auth-Token").value();
    if (token.empty() || std::stoul(token) == 0) {
        throw std::runtime_error("invalid token");
    }

    this->usager = jsonDocument["usager"].GetString();
    this->mot_de_passe = jsonDocument["mot_de_passe"].GetString();
    this->id = std::stoul(token);
}

