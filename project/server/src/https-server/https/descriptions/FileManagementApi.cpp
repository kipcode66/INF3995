#include "FileManagementApi.hpp"
#include "Admin.hpp"

#include <common/database/Database.hpp>
#include <common/rest/rest_utils.hpp>

#include <http-server/http/exception/InvalidTokenException.hpp>
#include <http-server/http/exception/MissingTokenException.hpp>

#include "https/exception/AuthenticationFailureException.hpp"

using namespace Pistache;

namespace elevation {

FileManagementApi::FileManagementApi(Rest::Description& desc, Logger& logger)
    : m_logger(logger)
{
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
            .route(desc.get("/file"))
            .bind(&FileManagementApi::getSuperviseurFile_, this)
            .hide();

    superviseurPath
            .route(desc.del("/chanson/:no"))
            .bind(&FileManagementApi::deleteSuperviseurChanson_, this)
            .hide();

    superviseurPath
            .route(desc.post("/inversion"))
            .bind(&FileManagementApi::postSuperviseurInversion_, this)
            .hide();
}

void FileManagementApi::getSuperviseurFile_(const Rest::Request& request,
                                            Http::ResponseWriter response) {
    // querying a param from the request object, by name
    std::thread([this](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        Pistache::Http::Code errorCode;
        std::string errorMessage;
        try {
            Admin requestAdmin = Admin::getAdminDataFromRequestToken(request);
            std::string serializedList = rest_utils::generateAllSongsAsViewedBy_(requestAdmin.getId(), true);

            std::ostringstream logMsg;
            logMsg << "The file list for admin \"" << requestAdmin.getId() << "\" was successfuly sent.";
            m_logger.log(logMsg.str());
            response.send(Pistache::Http::Code::Ok, serializedList);
            return;
        }
        catch (const MissingTokenException& e) {
            errorCode = Pistache::Http::Code::Bad_Request;
            errorMessage = e.what();
        }
        catch (const InvalidTokenException& e) {
            errorCode = Pistache::Http::Code::Unauthorized;
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
        response.send(errorCode, std::string{"getSuperviseurFile failed: "} + errorMessage);
    }, std::move(request), std::move(response)).detach();
}

void FileManagementApi::deleteSuperviseurChanson_(const Rest::Request& request,
                                                  Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteSuperviseurChanson called");
}

void FileManagementApi::postSuperviseurInversion_(const Rest::Request& request,
                                                  Http::ResponseWriter response) {
    std::thread([this](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        rapidjson::Document jsonDocument;
        jsonDocument.Parse(request.body().c_str());
        bool isValid = (jsonDocument.IsObject() &&
                        jsonDocument.HasMember("une") &&
                        jsonDocument.HasMember("autre") &&
                        jsonDocument["une"].IsNumber() &&
                        jsonDocument["autre"].IsNumber());
        if (!isValid) {
            response.send(Http::Code::Bad_Request, "Malformed request");
            return;
        }
        uint32_t first = jsonDocument["une"].GetUint();
        uint32_t second = jsonDocument["autre"].GetUint();
        if (first == second) {
            response.send(Http::Code::Ok, "order unchanged");
            return;
        }
        Pistache::Http::Code errorCode;
        std::string errorMessage;
        try {
            Admin requestAdmin = Admin::getAdminDataFromRequestToken(request);

            Database* db = Database::instance();
            Song_t song1 = db->getSongById(first);
            Song_t song2 = db->getSongById(first);
            if (song1.id == 0 || strlen(song1.path) == 0 || song2.id == 0 || strlen(song2.path) == 0) {
                m_logger.err("One or both songs are not in the queue");
                response.send(Http::Code::Conflict, "One or both songs are not in the queue");
                return;
            }

            db->swapSongs(&song1, &song2);

            std::ostringstream logMsg;
            logMsg << "The songs \"" << song1.title << "\" and \"" << song2.title << "\" were suscessfully swaped.";
            m_logger.log(logMsg.str());
            response.send(Pistache::Http::Code::Ok, "");
            return;
        }
        catch (const MissingTokenException& e) {
            errorCode = Pistache::Http::Code::Bad_Request;
            errorMessage = e.what();
        }
        catch (const InvalidTokenException& e) {
            errorCode = Pistache::Http::Code::Unauthorized;
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
        response.send(errorCode, std::string{"getSuperviseurFile failed: "} + errorMessage);
    }, std::move(request), std::move(response)).detach();
}

} // namespace elevation
