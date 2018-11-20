#include "FileManagementApi.hpp"
#include "Admin.hpp"

#include <common/database/Database.hpp>
#include <common/rest/rest_utils.hpp>

using namespace Pistache;

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
    try {
        Admin requestAdmin{request};
        Database* db = Database::instance();
        if (db->isAdminConnected(requestAdmin.getId())) {
            std::string serializedList = rest_utils::generateAllSongsAsViewedBy_(requestAdmin.getId(), true);

            std::ostringstream logMsg;
            logMsg << "The file list for admin \"" << requestAdmin.getId() << "\" was successfuly sent.";
            m_logger.log(logMsg.str());
            response.send(Pistache::Http::Code::Ok, serializedList);
        }
        else {
            std::ostringstream errorMsg;
            errorMsg << "User \"" << requestAdmin.getId() << "\" is not an admin.";
            m_logger.err("getSuperviseurFile failed: " + errorMsg.str());
            response.send(Pistache::Http::Code::Forbidden, errorMsg.str());
        }
    }
    catch (const std::exception& e) {
        m_logger.err(std::string{"getSuperviseurFile failed: "} + e.what());
        response.send(Pistache::Http::Code::Forbidden, e.what());
        return;
    }
}, std::move(request), std::move(response)).detach();
}

void FileManagementApi::deleteSuperviseurChanson_(const Rest::Request& request,
                                                  Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteSuperviseurChanson called");
}

void FileManagementApi::postSuperviseurInversion_(const Rest::Request& request,
                                                  Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "postSuperviseurInversion called");
}

