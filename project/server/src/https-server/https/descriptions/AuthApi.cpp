
#include <rapidjson/document.h>
#include <future>

#include <database/Database.hpp>
#include <common/logger/Logger.hpp>

#include "misc/id_utils.hpp"
#include "Admin.hpp"
#include "AuthApi.hpp"

using namespace Pistache;
using namespace elevation;

AuthApi::AuthApi(Pistache::Rest::Description& desc, Logger& logger)
    : m_logger(logger)
{
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
            .route(desc.post("/login"))
            .bind(&AuthApi::superviseurLogin_, this)
            .hide();

    superviseurPath
            .route(desc.post("/logout"))
            .bind(&AuthApi::superviseurLogout_, this)
            .hide();

    superviseurPath
            .route(desc.post("/changement_mdp"))
            .bind(&AuthApi::postChangePassword_, this)
            .hide();
}

void AuthApi::superviseurLogin_(const Rest::Request& request, Http::ResponseWriter response) {
    std::ostringstream logMsg;
    try {
        Admin admin(request);
        if (admin.getUsername() != Database::ADMIN_NAME) {
            logMsg << "Could not login the admin. Received wrong login";
            m_logger.err(logMsg.str());
            response.send(Http::Code::Bad_Request, "Wrong login");
            return;
        }
        std::async([&]() {
            Database* db = Database::instance();
            if (db->isAdminConnected(admin.getId())) {
                logMsg << "Could not Login Admin. Admin with token \"" << admin.getId() << "\" is already connected.";
                m_logger.err(logMsg.str());
                response.send(Http::Code::Bad_Request, "Admin already connected with this token");
                return;
            }
            auto saltAndPasswordHash = db->getSaltAndHashedPasswordByLogin(admin.getUsername().c_str());
            std::string salt = std::get<0>(saltAndPasswordHash);
            std::string hash = std::get<1>(saltAndPasswordHash);
            std::string passwordHash = elevation::id_utils::generateMd5Hash(admin.getMotDePasse(), salt);
            if (passwordHash == hash) {
                db->connectAdmin(admin.getUsername().c_str(), admin.getId());
                logMsg << "Successfuly logged in admin with token \"" << admin.getId() << "\".";
                m_logger.log(logMsg.str());
                response.send(Http::Code::Ok, "Connexion successful");
                return;
            } else {
                logMsg << "Could not Login admin with token \"" << admin.getId() << "\". Received wrong password";
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

void AuthApi::superviseurLogout_(const Rest::Request& request, Http::ResponseWriter response) {
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

void AuthApi::postChangePassword_(const Rest::Request& request, Http::ResponseWriter response) {
    try {
        rapidjson::Document jsonDocument;
        jsonDocument.Parse(request.body().c_str());
        bool isValid = (jsonDocument.IsObject() &&
                        jsonDocument.HasMember("ancien") &&
                        jsonDocument.HasMember("nouveau"));
        if (!isValid) {
            response.send(Http::Code::Bad_Request, "Malformed request");
            return;
        }
        std::string ancien(jsonDocument["ancien"].GetString());
        std::string nouveau(jsonDocument["nouveau"].GetString());
        if (nouveau == ancien) {
            response.send(Http::Code::Ok, "password unchanged");
            return;
        }

        Database* db = Database::instance();
        auto saltAndPasswordHash = db->getSaltAndHashedPasswordByLogin(Database::ADMIN_NAME);
        std::string salt = std::get<0>(saltAndPasswordHash);
        std::string passHashFromDB = std::get<1>(saltAndPasswordHash);
        if (elevation::id_utils::generateMd5Hash(ancien, salt) == passHashFromDB) {
            db->setAdminPassword(nouveau);
            response.send(Http::Code::Ok, "password changed");
        } else {
            response.send(Http::Code::Bad_Request, "old password incorrect");
        }
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        response.send(Http::Code::Internal_Server_Error, e.what());
    }
    return;
}

