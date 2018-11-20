
#include <future>

#include <rapidjson/document.h>

#include <common/database/Database.hpp>
#include <common/logger/Logger.hpp>

#include <http-server/http/exception/InvalidTokenException.hpp>
#include <http-server/http/exception/MissingTokenException.hpp>

#include "misc/id_utils.hpp"
#include "Admin.hpp"
#include "AuthApi.hpp"

#include "https/exception/AuthenticationFailureException.hpp"

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
    Pistache::Http::Code errorCode;
    std::string errorMessage;
    std::async([&]() {
        try {
            Admin admin = Admin::extractAdminDataFromRequest(request);
            if (admin.getUsername() != Database::ADMIN_NAME) {
                throw AuthenticationFailureException{admin.getUsername(), admin.getMotDePasse()};
            }
            Database* db = Database::instance();
            if (db->isAdminConnected(admin.getId())) {
                std::ostringstream logMsg;
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
                std::ostringstream logMsg;
                logMsg << "Successfuly logged in admin with token \"" << admin.getId() << "\".";
                m_logger.log(logMsg.str());
                response.send(Http::Code::Ok, "Connexion successful");
            } else {
                throw AuthenticationFailureException{admin.getUsername(), admin.getMotDePasse()};
            }
            return;
        }
        catch (const BadRequestException& e) {
            errorCode = Pistache::Http::Code::Bad_Request;
            errorMessage = e.what();
        }
        catch (const InvalidTokenException& e) {
            errorCode = Pistache::Http::Code::Bad_Request;
            errorMessage = e.what();
        }
        catch (const AuthenticationFailureException& e) {
            errorCode = Pistache::Http::Code::Forbidden;
            errorMessage = e.what();
        }
        catch (const std::exception& e) {
            errorCode = Http::Code::Internal_Server_Error;
            errorMessage = e.what();
        }
        m_logger.err(std::string{"superviseurLogin failed: "} + errorMessage);
        response.send(errorCode, errorMessage);
    });
}

void AuthApi::superviseurLogout_(const Rest::Request& request, Http::ResponseWriter response) {
    std::async([&]() {
        Pistache::Http::Code errorCode;
        std::string errorMessage;
        try {
            Admin requestAdmin = Admin::getAdminDataFromRequestToken(request);
            Database* db = Database::instance();

            db->disconnectAdmin(requestAdmin.getId());

            std::ostringstream logMsg;
            logMsg << "Admin with token \"" << requestAdmin.getId() << "\" was successfuly logged out.";
            m_logger.log(logMsg.str());
            response.send(Http::Code::Ok, "Logout successful");
            return;
        }
        catch (const MissingTokenException& e) {
            errorCode = Pistache::Http::Code::Bad_Request;
            errorMessage = e.what();
        }
        catch (const InvalidTokenException& e) {
            errorCode = Pistache::Http::Code::Forbidden;
            errorMessage = e.what();
        }
        catch (const AuthenticationFailureException& e) {
            errorCode = Pistache::Http::Code::Unauthorized;
            errorMessage = e.what();
        }
        catch (const std::exception& e) {
            errorCode = Pistache::Http::Code::Internal_Server_Error;
            errorMessage = e.what();
        }
        m_logger.err(std::string{"superviseurLogout failed: "} + errorMessage);
        response.send(errorCode, errorMessage);
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

