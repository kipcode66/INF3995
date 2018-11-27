#include <thread>
#include <future>

#include "FileManagementApi.hpp"
#include "Admin.hpp"

#include <common/database/Database.hpp>
#include <common/rest/rest_utils.hpp>

#include <http-server/http/exception/InvalidTokenException.hpp>
#include <http-server/http/exception/MissingTokenException.hpp>

#include "https/exception/AuthenticationFailureException.hpp"

using namespace Pistache;
using namespace elevation;


namespace elevation {

FileManagementApi::FileManagementApi(Rest::Description& desc, Logger& logger, FileCache& cache)
    : m_logger(logger)
    , m_cache(cache)
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
    std::thread([this](const Rest::Request& request, Http::ResponseWriter response) {
        Database* db = Database::instance();
        uint32_t songId;
        std::string errorMessage;
        Pistache::Http::Code errorCode;
        try {
            songId = request.param(":no").as<uint32_t>();
        }
        catch (const std::runtime_error& e) {
            response.send(Pistache::Http::Code::Bad_Request, e.what());
            return;
        }
        try {
            Admin requestAdmin = Admin::getAdminDataFromRequestToken(request);
            Song_t song = db->getSongById(songId);

            std::ostringstream logMessage;
            if (song.id != 0) {
                db->removeSongByAdmin(songId);
                m_cache.deleteFile(song.path);

                logMessage << "{ Administrator }"
                           << " Removed MP3 \"" << song.title
                           << "\" of length " << song.duration;
                m_logger.log(logMessage.str());
                response.send(Pistache::Http::Code::Ok);
                return;
            }
            else {
                logMessage << "{ Administrator }"
                           << " tried to remove nonexistant song of id "
                           << songId;
                m_logger.err(logMessage.str());
                response.send(Pistache::Http::Code::Method_Not_Allowed);
                return;
            }
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
            response.send(Pistache::Http::Code::Forbidden, e.what());
            return;
        }
        response.send(errorCode, std::string{"getSuperviseurFile failed: "} + errorMessage);
    }, std::move(request), std::move(response)).detach();
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
            Song_t song2 = db->getSongById(second);
            std::vector<Song_t> songs = db->getAllPlayableSongs();
            bool isFirst = false;
            if (songs.size() > 0) {
                isFirst = songs[0].id == song1.id || songs[0].id == song2.id;
            }
            if (song1.id == 0 || strlen(song1.path) == 0 || song2.id == 0 || strlen(song2.path) == 0 || isFirst) {
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
